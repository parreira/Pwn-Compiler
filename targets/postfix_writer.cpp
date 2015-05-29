// $Id: postfix_writer.cpp,v 1.76 2015/05/20 10:35:29 ist175814 Exp $ -*- c++ -*-
#include <string>
#include <sstream>
#include <typeinfo>
#include <vector>
#include "targets/type_checker.h"
#include "targets/local_variable_counter.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------


void pwn::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  declareVariable(node->value());
}

void pwn::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  declareVariable(node->value());
}


void pwn::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  declareVariable(node->value());
}


//---------------------------------------------------------------------------

void pwn::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::neg,node->type(),node->argument(),lvl);
}

void pwn::postfix_writer::do_not_node(pwn::not_node * const node, int lvl) {
  //so.. 0 equals false and any other means true... so..
  // not can't transform anyother number to 0 !
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl+1); // determine the value
  _pf.NOT();  //i compare the argument not with false not !!
  _pf.INT(0);
  _pf.NOT();
  _pf.EQ(); //very safe, if i dont do this i cant get ~(1 > 0) => 0
  //this way.. what happens is that 0 == false 1 == true very very safe...
  node->type(new basic_type(4,basic_type::TYPE_INT));
}

void pwn::postfix_writer::do_identity_node(pwn::identity_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this,lvl);
}

void pwn::postfix_writer::do_memory_reservation_node(pwn::memory_reservation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  setSegm(Segment::text); //i need to do this or global pointers doesn't work..
  if(!_global)_pf.INT(8); // 8 bytes
  else _pf.CONST(8);
  node->argument()->accept(this,lvl); 
  _pf.MUL();		//multiply 8 bytes for the integer expression index
  _pf.ALLOC();		//allocate 8*expr memory slots
  _pf.SP();		//push the stackpointer !
  setOldSegmn();	//continue to do whatever..

}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::add,node->type(),node->left(),node->right(),lvl);
}
void pwn::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::sub,node->type(),node->left(),node->right(),lvl);
}
void pwn::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::mul,node->type(),node->left(),node->right(),lvl);
}
void pwn::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::div,node->type(),node->left(),node->right(),lvl);
}
void pwn::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::mod,node->type(),node->left(),node->right(),lvl);
  
}
void pwn::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::lt,node->type(),node->left(),node->right(),lvl);
  node->type(new basic_type(4,basic_type::TYPE_INT));
}
void pwn::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::le,node->type(),node->left(),node->right(),lvl);
  node->type(new basic_type(4,basic_type::TYPE_INT));
}
void pwn::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::ge,node->type(),node->left(),node->right(),lvl);
  node->type(new basic_type(4,basic_type::TYPE_INT));
}
void pwn::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::gt,node->type(),node->left(),node->right(),lvl);
  node->type(new basic_type(4,basic_type::TYPE_INT));
}
void pwn::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::ne,node->type(),node->left(),node->right(),lvl);
  node->type(new basic_type(4,basic_type::TYPE_INT));
}
void pwn::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  convertDoubleInt(Op::eq,node->type(),node->left(),node->right(),lvl);
  node->type(new basic_type(4,basic_type::TYPE_INT));
}



void pwn::postfix_writer::do_or_node(pwn::or_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  int lbl1 = ++_lbl;
  node->left()->accept(this,lvl);
  _pf.DUP();
  _pf.JNZ(mklbl(lbl1));
  node->right()->accept(this, lvl);
  _pf.OR();
  _pf.LABEL(mklbl(lbl1)); 
}

void pwn::postfix_writer::do_and_node(pwn::and_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  int lbl1 = ++_lbl;
  node->left()->accept(this,lvl);
  _pf.DUP();
  _pf.JZ(mklbl(lbl1));
  node->right()->accept(this, lvl);
  _pf.AND();
  _pf.LABEL(mklbl(lbl1));
}

void pwn::postfix_writer::do_index_node(pwn::index_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  bool isFirstIndex = !_indexing;
  _indexing = true;
  node->lval()->accept(this, lvl +2); //VISIT LEFT NODE!
  node->rval()->accept(this, lvl +2); //VISIT THE INDEX INTEGER EXPRESSION
  _pf.INT(8); //push 8 bytes
  _pf.MUL();  // multiply 8 bytes for the index integer expression
  _pf.ADD();  //add it to the address of the left node
  if(isFirstIndex){
    _indexing = false;
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
    CHECK_TYPES(_compiler, _symtab, node);
    node->lvalue()->accept(this, lvl+1); //visit the lvalue node
    //CONSTANT GLOBAL STRING !!!
    //SPECIAL ONE CASE !!
    std::shared_ptr<pwn::symbol> s = _symtab.find_local(node->lvalue()->value());
    if(s == nullptr){
      s = _symtab.find(node->lvalue()->value());
    }
    if(s!= nullptr){
       //we dont load string const .. because a string is a pointer.. niceee
      if(!(s->isConst()  && node->lvalue()->type()->name() == basic_type::TYPE_STRING)){
	  doLoad(node->lvalue()->type());
      }
    }
    else{
      doLoad(node->lvalue()->type());
    }
    if(_indexing && node->lvalue()->name() == "index_node"){
      _pf.D2I();
    }
}


//---------------------------------------------------------------------------

void pwn::postfix_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node); 
  std::shared_ptr<pwn::symbol> s = _symtab.find_local(node->lvalue()->value());
  if(s == nullptr){
    s = _symtab.find(node->lvalue()->value());
  }
  s->assigning(true); 
  if(_global){ //we are in global context...
    if(!s->isConst()){ //its not a constant
      node->lvalue()->accept(this, lvl +1); //we visit the lvalue first
      node->rvalue()->accept(this, lvl +1); // then we save the value
    }
    else{
      setSegm(Segment::rodata); 	//so its a constant
      node->rvalue()->accept(this, lvl +1); //value first
      node->lvalue()->accept(this, lvl +1);  //then the variable
    }
  }
  else{
      node->rvalue()->accept(this, lvl +1); 
      //HUMMMM... %x = 3 ; well we convert 3 to double and store it.. by using I2D!
      //INTEGER 2 DOUBLE
      if(node->lvalue()->type()->size() > node->rvalue()->type()->size()){
	  node->rvalue()->type(node->lvalue()->type());
	  _pf.I2D();
      }
      else{
	if(node->lvalue()->type()->size() < node->rvalue()->type()->size() ){
	  node->rvalue()->type(node->lvalue()->type());
	  _pf.D2I();
	}
      }
      doDup(node->rvalue()->type()); //duplicate de value in the stack
      node->lvalue()->accept(this, lvl +1);
      doStore(node->lvalue()->type()); //store the value in the variable
    
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl +1); //EVALUATE THE EXPRESSION AS INSTRUCTION
  _pf.TRASH(node->argument()->type()->size()); //TRASH THE UNUSED VARIABLES
}

void pwn::postfix_writer::do_println_node(pwn::println_node * const node, int lvl) {
  node->argument()->accept(this, lvl+1); // determine the value to print
  //POINTERS ARE TYPES TOO DONT DISCRIMINATE
  if (node->argument()->type()->name() == basic_type::TYPE_INT
        || node->argument()->type()->name() == basic_type::TYPE_POINTER
    ) 
  {
    insertImport("printi");
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    insertImport("prints");
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) 
  {
    insertImport("printd");
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  insertImport("println");
  _pf.CALL("println"); // print a newline
}

void pwn::postfix_writer::do_print_node(pwn::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl); // determine the value to print
  //POINTERS ARE TYPES TOO DONT DISCRIMINATE
  if (node->argument()->type()->name() == basic_type::TYPE_INT
     || node->argument()->type()->name() == basic_type::TYPE_POINTER
  ) {
    insertImport("printi");
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    insertImport("printd");
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    insertImport("prints");
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_read_node(pwn::read_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if(node->type()->name() == basic_type::TYPE_INT){
    insertImport("readi"); //push extern , so we dont declare it more than once
    _pf.CALL("readi");
    _pf.PUSH();
  }
  else{
    insertImport("readd");
    _pf.CALL("readd");
    _pf.DPUSH();
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  int lbegin = ++_lbl;
  int lcond = ++_lbl;
  int lfinish= ++_lbl;
  _stop_labels.push_back(mklbl(lfinish));
  _next_labels.push_back(mklbl(lbegin));
  
  if(node->init() != NULL){
    node->init()->accept(this, lvl);
  }
  _pf.LABEL(mklbl(lcond));
  if(node->condition()!= NULL){
     node->condition()->accept(this, lvl); 
  }
  _pf.JZ(mklbl(lfinish));
  if(node->block()!= NULL){
    node->block()->accept(this, lvl + 2);
  }
  _pf.LABEL(mklbl(lbegin)); //next ...
  if(node->increment() != NULL){
    node->increment()->accept(this,lvl);
  }
  _pf.JMP(mklbl(lcond));
  _pf.LABEL(mklbl(lfinish));
  _stop_labels.pop_back();
  _next_labels.pop_back();
}


void pwn::postfix_writer::do_decl_func_node(pwn::decl_func_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  //well lets declare the parameters ... and the respective types !!!
  basic_type** argTypes = NULL;
  if(node->arguments() != NULL){ 
      argTypes = new basic_type*[node->arguments()->size()] ;
      variable_node * arg;
      for (size_t i = 0; i < node->arguments()->size(); i++) {
	arg = (variable_node*) node->arguments()->node(i);
	argTypes[i] = arg->type();     // let me save functions types , so i know what can i READ!
      }
  }
  //AND PASS THE TYPES OF PARAMETERS TO SYMBOL TABLES!!
  //INSERTING SYMBOL IN TABLE !!!
  std::shared_ptr<pwn::symbol> s 
	= std::make_shared<pwn::symbol>
	    (node->type(),false,node->isLocal(),node->isImport(),0 - node->type()->size() ,argTypes);
  _symtab.insert(""+node->lvalue(),s);
}

void pwn::postfix_writer::do_decl_var_node(pwn::decl_var_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if(node->isImport()){
    return;
  }
  int offset;
  std::string varname = node->value();
  std::shared_ptr<pwn::symbol> s = _symtab.find_local(varname);
  if(node->isGlobal()){ // GLOBAL VARIABLE
    s->label(mklbl(_lbl)); //saving the label in the symbol table
    if(!s->assigning()){ //noninicialized
      s->define(false); //we can declare that its not defined ..
      setSegm(Segment::bss);
      _pf.LABEL(varname); 	
      _pf.BYTE(node->type()->size()); //reserve some bytes...
    }
    else { 
      s->define(true);
      if(!node->isConst()){ // nonconstant inicialized
	setSegm(Segment::data); //DATA
	if(!node->isLocal()){ _pf.GLOBAL(varname, _pf.OBJ());}
	_pf.LABEL(varname);
	//visit r value now..
      }
      else{ 
	//rodata and visit r value
	 setSegm(Segment::rodata); //RODATA
	 if(!node->isLocal()){ _pf.GLOBAL(varname, _pf.OBJ());}
	 _pf.LABEL(varname);
	 _pf.ID(mklbl(_lbl));	//some way to say that this is a const
				//THERE IS NO WAY U CAN CHANGE THIS !
      }     
    }
  }
  else{ // LOCAL VARIABLE !!!
     if(s->assigning()){ //inicialized local variable
      s->define(true);
      local_decl_offset -= node->type()->size();
      s->offset(local_decl_offset);
      (new variable_node(node->lineno(),new std::string(varname)))->accept(this, lvl +1);
      return;
    }
    else{
      s->define(false);
      if(node->isFuncArgument()){ // is function argument 
	offset = local_arg_offset;
	local_arg_offset+= node->type()->size();
	s->offset(offset); 
      }
      else{ // simple local variable 4 sure
	local_decl_offset -= node->type()->size();
	s->offset(local_decl_offset);
      }
    }
  }
}

void pwn::postfix_writer::do_variable_node(pwn::variable_node * const node , int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  const std::string &id = node->value();
  std::shared_ptr<pwn::symbol> s = _symtab.find_local(id);
  if(s == nullptr){ //GLOBAL VARIABLE or FUNCTION ARGUMENT
    s=_symtab.find(id);    
    if(!s->isImport()){ //is this an import ?!?!
      if(s->isGlobalVar()){ //this is a global variable
	if(s->isConst()){ //constant...
	  _pf.ADDR(id);
	  _pf.LOAD(); // LOAD ID ...
	}
	else{
	  _pf.ADDR(id); //not a contant just get the address of the variable 
	}
      }      
      else{
	_pf.LOCAL(s->offset()); // RETURN VARIABLE !! OR FUNCTION ARGUMENT !!
      }
    }
    else{ //this is an import ...
      if(s->isConst()){
	_pf.ADDR(id);
	_pf.LOAD();
	insertImport(id);
      }
    }
  }
  else{ //LOCAL VARIABLE !!
     _pf.LOCAL(s->offset());  
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_node(cdk::if_node * const node, int lvl) {
  int lbl1;
  node->condition()->accept(this, lvl); //verificar condição if
  _pf.JZ(mklbl(lbl1 = ++_lbl));        // saltar com a condição
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//----------------------------------------------------------------------------

void pwn::postfix_writer::do_noob_node(pwn::noob_node * const node, int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  if(!_global)_pf.INT(0);  // safer this way i dont want to declare a double
  else _pf.CONST(0);
  
}

void pwn::postfix_writer::do_return_node(pwn::return_node * const node, int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  if(this_function == NULL){ //hmm we are not in a function
    //ERRO
  }
  popFuncReturn(this_function->type());
}

void pwn::postfix_writer::do_next_node(pwn::next_node * const node, int lvl){
  int n_next = 1; //1
  int n_repeats = _next_labels.size();   //3  --> [2]
  if(node->argument()!= NULL){
    n_next = node->argument()->value(); //2
  }
  if(n_repeats >= n_next){
    _pf.JMP(_next_labels[n_repeats-n_next]);
  }
  else{
    //ERRO 
  }
}

void pwn::postfix_writer::do_stop_node(pwn::stop_node * const node, int lvl){
  int n_stop = 1;
  int n_repeats = _stop_labels.size();
  if(node->argument()!= NULL){
    n_stop = node->argument()->value(); //2
  }
  if(n_repeats >= n_stop){
    _pf.JMP(_stop_labels[n_repeats-n_stop]);
  }
  else{
    //ERRO 
  }
}

void pwn::postfix_writer::do_addressing_node(pwn::addressing_node * const node , int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this , lvl +1); //visit the variable but we dont load it
					    //so easy
}

void pwn::postfix_writer::do_def_func_node(pwn::def_func_node * const node , int lvl){
  node->func_decl()->accept(this, lvl +1); //let me declare the function first ...
  CHECK_TYPES(_compiler, _symtab, node);
  const std::string &name = node->func_decl()->lvalue();
  std::string funcName = name;
  if(name == "pwn"){ //must declare _main ..so it compiles
      funcName = std::string("_main");
  }
  else{
    if(name == "_main"){
    }
  }

  setSegm(Segment::text); //text
  local_variable_counter counter(node); //the local variables counter
  _global = false; //we are not declaring global variables...
  setLocalOffSets(); //defult offsets (arguments and local variables)
  this_function= node->func_decl(); //let me save the current function..
  _symtab.push(); // push a context so we can use find_local
  if(!node->func_decl()->isLocal()){ _pf.GLOBAL(funcName, _pf.FUNC());}
  _pf.LABEL(funcName);
  _pf.ENTER(counter.size()); // the size of local variables in BYTES !!
  if(node->func_decl()->arguments() != NULL){
    node->func_decl()->arguments()->accept(this, lvl +1); // do we have arguments?
  }
  saveReturnVariable(node->func_decl()->type(), node->func_decl()->literal(),lvl);
  node->block()->accept(this, lvl+1); //there is allways a block  
  popFuncReturn(this_function->type()); //pop the return !!
  if(funcName == "_main"){ //a way to ensure that only one function uses calls extern function
    externImports(); //extern all imports used
    externUnDefSymbols(); //extern all symbols undefined 
  }
  _global = true; //out of a function
  _symtab.pop(); // context is over bye bye local variables
}

void pwn::postfix_writer::do_call_func_node(pwn::call_func_node * const node , int lvl){
  //CHECK FOR PWN VS _MAIN !!! 
  CHECK_TYPES(_compiler, _symtab, node);
  if(node->arguments() != NULL){
    node->arguments()->accept(this, lvl +1);
  }
  std::shared_ptr<pwn::symbol> s = _symtab.find(node->identifier());
  if(!s->defined()){
    insertUndefSymbol(node->identifier()); // guardar simbolos que ainda nao foram definidos
  }
  _pf.CALL(node->identifier());
  node->type(s->type());
  doFuncReturn(node->type());  
}

void pwn::postfix_writer::do_block_node(pwn::block_node * const node , int lvl){
  _symtab.push();
  if(node->stmts() != NULL){
    node->stmts()->accept(this,lvl + 1);
  }
  if(node->instrs() != NULL){
    node->instrs()->accept(this, lvl +1);
  }
  _symtab.pop();
}

void pwn::postfix_writer::do_apply_from_to_node(pwn::apply_from_to_node * const node , int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  int lbl1 = ++_lbl;
  int lbl2 = ++_lbl;
  std::shared_ptr<pwn::symbol> s = _symtab.find_local(node->function());
  if(s == nullptr){ //GLOBAL VARIABLE or FUNCTION ARGUMENT
    s=_symtab.find(node->function()); 
  }
  basic_type * funcType = s->getArgsTypes()[0]; //get the first argument xDD
  pwn::call_func_node * funcCall = new pwn::call_func_node(node->lineno(),new std::string(node->function()),NULL);
  
  //declare a variable to engage in the vector
  local_decl_offset -= 4;
  int i = local_decl_offset;
  //engage the index variable
 
  //save the index.
  node->lvalue()->accept(this, lvl);
  if(node->lvalue()->type()->name()== basic_type::TYPE_DOUBLE){
    _pf.D2I();
  }
  _pf.LOCAL(i);
  _pf.STORE();
  
  //begin the cicle
  _pf.LABEL(mklbl(lbl1));
  _pf.LOCAL(i); // index 
  _pf.LOAD();
  _pf.DUP();
  node->rvalue()->accept(this, lvl);
  if(node->rvalue()->type()->name()== basic_type::TYPE_DOUBLE){
    _pf.D2I();
  }
  _pf.LE();
  _pf.JZ(mklbl(lbl2));
  
  //get the index address
  node->lval()->accept(this,lvl);
  _pf.LOAD();
  
  //get the index increment
  _pf.LOCAL(i); //index number
  _pf.LOAD();	//index 
  
  _pf.INT(8);	//8 bytes 
  _pf.MUL();	//position x 8bytes
  _pf.ADD();	//add to memory position
  
  //PUT THE PARAMETER IN THE STACK !!
  _pf.DLOAD(); //get the double
  
  //check the function argument type...
  if(funcType->name() != basic_type::TYPE_DOUBLE){
    _pf.D2I();
  }
  //CALL THE FUNCTION
  funcCall->accept(this, lvl);
  
  //INCREMENT
  _pf.LOCAL(i); //THE ADDRESS OF THE VARIABLE
  _pf.LOAD(); //GET THE VALUE STORED
  _pf.INT(1); //add one
  _pf.ADD();  //add
  _pf.LOCAL(i); //THE ADDRESS OF THE VARIABLE
  _pf.STORE();
  _pf.JMP(mklbl(lbl1));//CHECK AGAIN THE INDEX
  _pf.LABEL(mklbl(lbl2));
}



