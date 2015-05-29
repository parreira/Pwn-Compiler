// $Id: local_variable_counter.cpp,v 1.7 2015/05/04 01:34:12 ist175814 Exp $ -*- c++ -*-
#include <string>
#include "targets/local_variable_counter.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void pwn::local_variable_counter::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void pwn::local_variable_counter::do_def_func_node(pwn::def_func_node * const node, int lvl) {
  //size of return variable 
  _counter += node->func_decl()->type()->size();
  node->block()->accept(this, lvl + 4);

}

void pwn::local_variable_counter::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  if(node->init()!=NULL){
    node->init()->accept(this, lvl + 4);
  }
  if(node->block()!= NULL){node->block()->accept(this, lvl + 4);}
}

void pwn::local_variable_counter::do_if_else_node(cdk::if_else_node * const node, int lvl){
  if(node->thenblock() != NULL){
    node->thenblock()->accept(this, lvl +2);
  }
  if(node->elseblock() != NULL){
    node->elseblock()->accept(this, lvl +2);
  }
}

void pwn::local_variable_counter::do_if_node(cdk::if_node * const node, int lvl){
  if(node->block() != NULL){
    node->block()->accept(this, lvl +2);
  }
}


void pwn::local_variable_counter::do_decl_var_node(pwn::decl_var_node * const node , int lvl){
  _counter += node->type()->size(); //humm some variable to add size..
  //well this is a visitor.. so this IS ALLWAYS CALLED AFTER A def_func_node
  //THIS WILL NEVER BE A GLOBAL VARIABLE IF U FOLOW THIS RULES !
  //i could do if(node->isGLobalVar()) but its just a waste of time
}

void pwn::local_variable_counter::do_block_node(pwn::block_node * const node , int lvl){
  if(node->stmts()!= NULL){node->stmts()->accept(this, lvl + 2);}
  if(node->instrs()!= NULL){node->instrs()->accept(this,lvl + 2);}
}

void pwn::local_variable_counter::do_assignment_node(pwn::assignment_node * const node, int lvl){
  node->lvalue()->accept(this, lvl +2); //this could be a variable declaration
}

