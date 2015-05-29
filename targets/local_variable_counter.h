// $Id: local_variable_counter.h,v 1.5 2015/05/18 02:53:44 ist175814 Exp $
#ifndef __PWN_SEMANTICS_COUNTER_H__
#define __PWN_SEMANTICS_COUNTER_H__

#include <cdk/basic_target.h>
#include <cdk/symbol_table.h>
#include <cdk/ast/basic_node.h>
#include <cdk/compiler.h>
#include <cdk/emitters/postfix_ix86_emitter.h>
#include "targets/postfix_writer.h"
#include "targets/symbol.h"

namespace pwn {

  class local_variable_counter : public basic_ast_visitor {
    size_t _counter; // bytes
    
  public:
    inline local_variable_counter(cdk::basic_node * const node) :
       basic_ast_visitor(NULL),_counter(0)  {
	 node->accept(this,0);
    }
    inline size_t size(){
      return _counter;
    }
  
  public:
    void do_sequence_node(cdk::sequence_node * const node, int lvl);

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl){}
    void do_string_node(cdk::string_node * const node, int lvl){}
    void do_double_node(cdk::double_node * const node, int lvl){}
  public:
    void do_neg_node(cdk::neg_node * const node, int lvl){}
    void do_not_node(pwn::not_node * const node, int lvl){}
    void do_identity_node(pwn::identity_node * const node, int lvl){}
    void do_memory_reservation_node(pwn::memory_reservation_node * const node, int lvl){}

  public:
    void do_add_node(cdk::add_node * const node, int lvl){}
    void do_sub_node(cdk::sub_node * const node, int lvl){}
    void do_mul_node(cdk::mul_node * const node, int lvl){}
    void do_div_node(cdk::div_node * const node, int lvl){}
    void do_mod_node(cdk::mod_node * const node, int lvl){}
    void do_lt_node(cdk::lt_node * const node, int lvl){}
    void do_le_node(cdk::le_node * const node, int lvl){}
    void do_ge_node(cdk::ge_node * const node, int lvl){}
    void do_gt_node(cdk::gt_node * const node, int lvl){}
    void do_ne_node(cdk::ne_node * const node, int lvl){}
    void do_eq_node(cdk::eq_node * const node, int lvl){}
    void do_or_node(pwn::or_node * const node, int lvl){}
    void do_and_node(pwn::and_node * const node, int lvl){}
    void do_index_node(pwn::index_node * const node, int lvl){}

  public:
    void do_repeat_node(pwn::repeat_node * const node, int lvl);
    void do_while_node(cdk::while_node * const node, int lvl){}
    void do_if_node(cdk::if_node * const node, int lvl);
    void do_if_else_node(cdk::if_else_node * const node, int lvl);
    void do_noob_node(pwn::noob_node * const node, int lvl){}
    void do_next_node(pwn::next_node * const node, int lvl){}
    void do_return_node(pwn::return_node * const node, int lvl){}
    void do_stop_node(pwn::stop_node * const node, int lvl){}

  public:
    void do_rvalue_node(pwn::rvalue_node * const node, int lvl){}

  public:
    void do_evaluation_node(pwn::evaluation_node * const node, int lvl){}
    void do_print_node(pwn::print_node * const node, int lvl){}
    void do_println_node(pwn::println_node * const node, int lvl){}
    void do_read_node(pwn::read_node * const node, int lvl){}
    void do_assignment_node(pwn::assignment_node * const node, int lvl);
    void do_addressing_node(pwn::addressing_node * const node , int lvl){}
    void do_decl_var_node(pwn::decl_var_node * const node , int lvl);
    void do_variable_node(pwn::variable_node * const node , int lvl){}
    void do_decl_func_node(pwn::decl_func_node * const node , int lvl){}
    void do_call_func_node(pwn::call_func_node * const node , int lvl){}
    void do_def_func_node(pwn::def_func_node * const node , int lvl);
    void do_block_node(pwn::block_node * const node , int lvl);
    void do_apply_from_to_node(pwn::apply_from_to_node * const node , int lvl){ _counter+=4;}
  };

} // pwn

#endif
