// $Id: type_checker.cpp,v 1.53 2015/05/20 10:32:31 ist175814 Exp $ -*- c++ -*-
#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void pwn::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void pwn::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}


void pwn::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

inline void pwn::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT
      || node->argument()->type()->name() != basic_type::TYPE_DOUBLE)
  {
    node->type(node->argument()->type());
  }else {
    //ERRO
  }
}

void pwn::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node,lvl);
}

void pwn::type_checker::do_not_node(pwn::not_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_INT
    ||  node->argument()->type()->name() == basic_type::TYPE_DOUBLE
  )
  {
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else {
    //ERRO
  }
}

void pwn::type_checker::do_identity_node(pwn::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void pwn::type_checker::do_memory_reservation_node(pwn::memory_reservation_node * const node, int lvl) {
  node->type(new basic_type(4,basic_type::TYPE_POINTER));
}

//---------------------------------------------------------------------------

inline void pwn::type_checker::processBinaryNumericExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  if(( node->right()->type()->name() == basic_type::TYPE_DOUBLE
    && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    || (node->left()->type()->name() == basic_type::TYPE_DOUBLE 
    && node->right()->type()->name() == basic_type::TYPE_INT)
    ||( node->right()->type()->name() == basic_type::TYPE_DOUBLE
    && node->left()->type()->name() == basic_type::TYPE_INT))
  {
      node->type(new basic_type(8,basic_type::TYPE_DOUBLE));
  }
  else{
    if((node->right()->type()->name() == basic_type::TYPE_INT
    && node->left()->type()->name() == basic_type::TYPE_INT)
      || (node->right()->type()->name() == basic_type::TYPE_POINTER
    && node->left()->type()->name() == basic_type::TYPE_INT)
      || (node->right()->type()->name() == basic_type::TYPE_INT
    && node->left()->type()->name() == basic_type::TYPE_POINTER)
    ){
      node->type(new basic_type(4,basic_type::TYPE_INT));
    }
    else{
      throw std::string("wrong type in binary expression");
    }
  }
}

inline void pwn::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  if(( node->right()->type()->name() == basic_type::TYPE_DOUBLE
    && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    || (node->left()->type()->name() == basic_type::TYPE_DOUBLE 
    && node->right()->type()->name() == basic_type::TYPE_INT)
    ||( node->right()->type()->name() == basic_type::TYPE_DOUBLE
    && node->left()->type()->name() == basic_type::TYPE_INT)){
      node->type(new basic_type(8,basic_type::TYPE_DOUBLE));
    }
  else{
    if(( node->right()->type()->name() == basic_type::TYPE_INT
    && node->left()->type()->name() == basic_type::TYPE_INT)){
      node->type(new basic_type(4,basic_type::TYPE_INT));
    }
    else{
      throw std::string("wrong type in binary expression");
    }
  }
}

void pwn::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryNumericExpression(node, lvl);
}
void pwn::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryNumericExpression(node, lvl);
}
void pwn::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryNumericExpression(node, lvl);
}
void pwn::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryNumericExpression(node, lvl);
}
void pwn::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  node->type(new basic_type(4,basic_type::TYPE_INT));
}
void pwn::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void pwn::type_checker::do_or_node(pwn::or_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void pwn::type_checker::do_and_node(pwn::and_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void pwn::type_checker::do_index_node(pwn::index_node * const node, int lvl) {
  if(node->lval()->name() == "rvalue_node"){ //i think i must do this .. if dont i never know
    pwn::rvalue_node * rval = (rvalue_node*)node->lval();
    if(rval->lvalue()->name() == "variable_node"  ){ //FINALLY VARIABLE WITH A NAME !
      pwn::variable_node * var = (variable_node*) rval->lvalue();
      node->value(var->value());
      node->lval()->accept(this, lvl +2);
      node->type(new basic_type(8,basic_type::TYPE_DOUBLE)); 
      //well they are 8bytes.. so.. lets just say this is a type double..
    }
    else{ // hmm matrix ? or more like f[0][0][0]... how to solve it ?
      node->lval()->accept(this, lvl +2);
      pwn::variable_node * var = (variable_node*) rval->lvalue();
      node->value(var->value());
      node->type(new basic_type(4,basic_type::TYPE_DOUBLE)); 
      //well this is the an index of index so.. lets just say its a pointer ...
    }
  }
  else{
    if(node->lval()->name() == "call_func_node"  ){ //FINALLY VARIABLE WITH A NAME !
      pwn::call_func_node * var = (call_func_node*) node->lval();
      node->value(var->identifier());
      node->lval()->accept(this, lvl +2);
      node->type(new basic_type(8,basic_type::TYPE_DOUBLE)); 
      //well they are 8bytes.. so.. lets just say this is a type double..
    }
    else{
      node->lval()->accept(this, lvl +2);
      node->type(node->lval()->type()); 
    }   
  }
}


//---------------------------------------------------------------------------

void pwn::type_checker::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}



//---------------------------------------------------------------------------

void pwn::type_checker::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl +2);
  node->rvalue()->accept(this, lvl +2);
  std::shared_ptr<pwn::symbol> s = _symtab.find_local(node->lvalue()->value());
  if(s == nullptr){ //GLOBAL VARIABLE or FUNCTION ARGUMENT
    s=_symtab.find(node->lvalue()->value());     
  }
  if(node->rvalue()->name() == "read_node"){
    node->rvalue()->type(node->lvalue()->type());
  }
  if(s->isConst()){
    //ERRO TRYING TO CHANGE A CONST !!!
  }
  s->assigning(true);
  s->define(true);
  node->type(s->type()); //assignment type..
  
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void pwn::type_checker::do_print_node(pwn::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void pwn::type_checker::do_println_node(pwn::println_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_read_node(pwn::read_node * const node, int lvl) {
  
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  if(node->init()!=NULL) node->init()->accept(this, lvl +1);
  if(node->condition()!= NULL)node->condition()->accept(this, lvl +1);
  if(node->increment()!=NULL)node->increment()->accept(this, lvl +1);
}


//---------------------------------------------------------------------------

void pwn::type_checker::do_noob_node(pwn::noob_node * const node, int lvl) {
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}

void pwn::type_checker::do_addressing_node(pwn::addressing_node * const node , int lvl){
  node->type(new basic_type(4,basic_type::TYPE_POINTER)); //size of address ?!
}

void pwn::type_checker::do_decl_func_node(pwn::decl_func_node * const node, int lvl) {
  if(node->type() == NULL){
    node->type(new basic_type(0,basic_type::TYPE_VOID));
  }
}

void pwn::type_checker::do_decl_var_node(pwn::decl_var_node * const node, int lvl) {
  if(_symtab.find_local(node->value())!= nullptr){
    //ERRO
  }
  else{
    if(node->isGlobal()){ // GLOBAL VARIABLE !!!!
      _symtab.insert(""+node->value(),
	std::make_shared<pwn::symbol>(node->type(),false,node->isLocal(),node->isImport(),node->isConst(),""));
    }
    else{ // LOCAL VARIABLE !!!
      _symtab.insert(""+node->value(),
	std::make_shared<pwn::symbol>(node->type(),false,0));
    }
  }
}

void pwn::type_checker::do_variable_node(pwn::variable_node * const node , int lvl){
    if(_symtab.find_local(node->value()) == nullptr){
      node->type(_symtab.find(node->value())->type());
    }
    else{
      node->type(_symtab.find_local(node->value())->type());
    }
}

void pwn::type_checker::do_def_func_node(pwn::def_func_node * const node, int lvl) {
  std::shared_ptr<pwn::symbol> symbol = _symtab.find(node->func_decl()->lvalue());
  if(symbol->defined()){ //someone defined this already
    //ERRO
  }
  else{
    symbol->define(true);
  }
}

void pwn::type_checker::do_apply_from_to_node(pwn::apply_from_to_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->rvalue()->accept(this, lvl);
  node->lval()->accept(this, lvl );
}

void pwn::type_checker::do_call_func_node(pwn::call_func_node * const node , int lvl){
  std::shared_ptr<pwn::symbol> s= _symtab.find(""+node->identifier());
  if (s == nullptr ) {
    //ERRO
  }
  else{
      if(node->arguments() != NULL){
	variable_node * arg;
	for (size_t i = 0; i < node->arguments()->size(); i++) {
	  arg = (variable_node*) node->arguments()->node(i);
	  arg->type(getBasicType((s->getArgsTypes())[i]));
	}
      } 
  }
  node->type(s->type());
}
void pwn::type_checker::do_block_node(pwn::block_node * const node, int lvl) {
  if(node->stmts() != NULL) node->stmts()->accept(this, lvl + 4);
  if(node->instrs() != NULL)node->instrs()->accept(this, lvl + 4);
}

