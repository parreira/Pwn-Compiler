// $Id: xml_writer.cpp,v 1.31 2015/05/18 02:53:44 ist175814 Exp $ -*- c++ -*-
#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void pwn::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  processSimple(node, lvl);
}

void pwn::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  processSimple(node, lvl);
}

void pwn::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  processSimple(node, lvl);
}

//---------------------------------------------------------------------------

inline void pwn::xml_writer::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void pwn::xml_writer::do_not_node(pwn::not_node * const node, int lvl) {
  processUnaryExpression(node, lvl);  
}

void pwn::xml_writer::do_identity_node(pwn::identity_node * const node, int lvl) {
    processUnaryExpression(node, lvl);  
}

void pwn::xml_writer::do_memory_reservation_node(pwn::memory_reservation_node * const node, int lvl) {
    processUnaryExpression(node, lvl);  
}

//---------------------------------------------------------------------------

inline void pwn::xml_writer::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void pwn::xml_writer::do_or_node(pwn::or_node * const node, int lvl) {
  processBinaryExpression(node,lvl);
}

void pwn::xml_writer::do_and_node(pwn::and_node * const node, int lvl) {
  processBinaryExpression(node,lvl);
}

void pwn::xml_writer::do_index_node(pwn::index_node * const node, int lvl) {
  openTag(node, lvl);
  
  openTag("lvalue_node",lvl+2);
  node->lval()->accept(this, lvl + 4 );
  closeTag("lvalue_node",lvl+2);
  openTag("index",lvl+2);
  node->rval()->accept(this, lvl + 4);
  closeTag("index",lvl+2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}


//---------------------------------------------------------------------------

void pwn::xml_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  openTag("rvalue", lvl + 2);
  node->rvalue()->accept(this, lvl + 4);
  closeTag("rvalue", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_def_func_node(pwn::def_func_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->func_decl()->accept(this, lvl +4);
  if(node->block()!= NULL){node->block()->accept(this, lvl + 4);}
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_print_node(pwn::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_println_node(pwn::println_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_read_node(pwn::read_node * const node, int lvl) {
  openTag(node, lvl);
  //node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("init", lvl + 2);
  if(node->init()!=NULL){
    node->init()->accept(this, lvl + 4);
  }
  closeTag("init", lvl + 2);
  openTag("condition", lvl + 2);
  if(node->condition()!=NULL){
    node->condition()->accept(this, lvl + 4);
  }
  closeTag("condition", lvl + 2);
  
  openTag("increment", lvl + 2);
  if(node->increment()!=NULL){
    node->increment()->accept(this, lvl + 4);
  }
  closeTag("increment", lvl + 2);
  
  if(node->block()!= NULL){node->block()->accept(this, lvl + 4);}
  closeTag(node, lvl);
}

void pwn::xml_writer::do_while_node(cdk::while_node * const node, int lvl) {
  //while does not is good in this language
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_if_node(cdk::if_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  if(node->condition() != NULL){node->condition()->accept(this, lvl + 4);}
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  if(node->block()!= NULL){node->block()->accept(this, lvl + 4);}
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  if(node->condition() != NULL){node->condition()->accept(this, lvl + 4);}
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  if(node->thenblock() != NULL){node->thenblock()->accept(this, lvl + 4);}
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  if(node->elseblock() != NULL){node->elseblock()->accept(this, lvl + 4);}
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}



void pwn::xml_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
    openTag(node, lvl);
    closeTag(node, lvl);
}
void pwn::xml_writer::do_next_node(pwn::next_node * const node, int lvl) {
    int nJMP = 0;
    if(node->argument()!= NULL){
      nJMP =  node->argument()->value();
    }
    os() << std::string(lvl, ' ') << "<next_node size='" << nJMP << "'>" << std::endl;
    closeTag(node, lvl);
}
void pwn::xml_writer::do_return_node(pwn::return_node * const node, int lvl) {
    openTag(node, lvl);
    closeTag(node, lvl);
}

void pwn::xml_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
    int nJMP = 0;
    if(node->argument()!= NULL){
      nJMP =  node->argument()->value();
    }
    os() << std::string(lvl, ' ') << "<stop_node size='" << nJMP << "'>" << std::endl;
    closeTag(node, lvl);
}

void pwn::xml_writer::do_decl_var_node(pwn::decl_var_node * const node , int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
      os() << std::string(lvl, ' ') << "<decl_var_node "
				    << "variable_name='" <<  node->value() << "' " 
				    << "type='" << typeName(node->type()) <<"' "
				    << "const='" << node->isConst() << "' "
				    << "local='" << node->isLocal() <<"' "
				    << "import='" << node->isImport() <<"' "
				    << "isFuncArg='" << node->isFuncArgument() <<"' "
				    << ">" << "\n";
      closeTag(node, lvl);
}
void pwn::xml_writer::do_variable_node(pwn::variable_node * const node , int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  os() << std::string(lvl, ' ') << "<variable_node "
  				    << "variable_name='" <<  node->value() << "' "
				    << ">" << "\n";
  closeTag(node, lvl);			 
}
void pwn::xml_writer::do_decl_func_node(pwn::decl_func_node * const node , int lvl){
  
        os() << std::string(lvl, ' ') << "<decl_func_node "
				    << "function_name='" <<  node->lvalue() << "' " 
				    << "type='" << typeName(node->type()) <<"' "
				    << "local='" << node->isLocal() <<"' "
				    << "import='" << node->isImport() <<"' "
				    << ">" << "\n";
	openTag("default_return", lvl + 2);
	if(node->literal() != NULL){
	  node->literal()->accept(this,lvl + 4);
	}
	closeTag("default_return", lvl + 2);
      if(node->arguments() != NULL){
	openTag("arguments", lvl + 2);
	node->arguments()->accept(this,lvl + 4);
	closeTag("arguments", lvl + 2);
      }
      closeTag(node, lvl);
}
void pwn::xml_writer::do_call_func_node(pwn::call_func_node * const node , int lvl){
  CHECK_TYPES(_compiler, _symtab, node);	
       os() << std::string(lvl, ' ') << "<call_func_node "
				    << "function_name='" <<  node->identifier() << "' " 
				    << ">" << "\n";
      if(node->arguments() != NULL){
	openTag("arguments", lvl + 2);
	node->arguments()->accept(this,lvl + 4);
	closeTag("arguments", lvl + 2);
      }
      closeTag(node, lvl);
  
}
void pwn::xml_writer::do_addressing_node(pwn::addressing_node * const node , int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  	openTag("address", lvl + 2);
	node->argument()->accept(this,lvl + 4);
	closeTag("address", lvl + 2);
}
void pwn::xml_writer::do_block_node(pwn::block_node * const node , int lvl){
  openTag("block", lvl );
  if(node->stmts()!= NULL){node->stmts()->accept(this, lvl + 2);}
  if(node->instrs()!= NULL){node->instrs()->accept(this,lvl + 2);}
  closeTag("block", lvl );

}








