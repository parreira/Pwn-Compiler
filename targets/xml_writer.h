// $Id: xml_writer.h,v 1.22 2015/05/19 18:18:24 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_XMLWRITER_H__
#define __PWN_SEMANTICS_XMLWRITER_H__

#include <string>
#include <iostream>
#include <cdk/ast/basic_node.h>
#include <cdk/symbol_table.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace pwn {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class xml_writer: public basic_ast_visitor {
    cdk::symbol_table<pwn::symbol> &_symtab;

  public:
    xml_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<pwn::symbol> &symtab) :
        basic_ast_visitor(compiler), _symtab(symtab) {
    }
    
    

  public:
    ~xml_writer() {
      os().flush();
    }

  private:
    inline void openTag(const std::string &tag, int lvl) {
      os() << std::string(lvl, ' ') + "<" + tag + ">" << std::endl;
    }
    inline void openTag(const cdk::basic_node *node, int lvl) {
      openTag(node->name(), lvl);
    }
    inline void closeTag(const std::string &tag, int lvl) {
      os() << std::string(lvl, ' ') + "</" + tag + ">" << std::endl;
    }
    inline void closeTag(const cdk::basic_node *node, int lvl) {
      closeTag(node->name(), lvl);
    }
    inline std::string typeName(basic_type * type){
      if(type == NULL){return "void";}
      switch(type->name()){
	case basic_type::TYPE_INT: return "integer";
	case basic_type::TYPE_DOUBLE: return "real";
	case basic_type::TYPE_STRING: return "string";
	case basic_type::TYPE_POINTER: return ("pointer");
      }
      return "undefined";
      
    }

  public:
    void do_sequence_node(cdk::sequence_node * const node, int lvl);

  protected:
    template<typename T>
    void processSimple(cdk::simple_value_node<T> * const node, int lvl) {
      os() << std::string(lvl, ' ') << "<" << node->name() << ">" << node->value() << "</" << node->name() << ">" << std::endl;
    }

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);
    void do_double_node(cdk::double_node * const node, int lvl);
    
  protected:
    void processUnaryExpression(cdk::unary_expression_node * const node, int lvl);

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);
    void do_not_node(pwn::not_node * const node, int lvl);
    void do_identity_node(pwn::identity_node * const node, int lvl);
    void do_memory_reservation_node(pwn::memory_reservation_node * const node, int lvl);

  protected:
    void processBinaryExpression(cdk::binary_expression_node * const node, int lvl);

  public:
    void do_add_node(cdk::add_node * const node, int lvl);
    void do_sub_node(cdk::sub_node * const node, int lvl);
    void do_mul_node(cdk::mul_node * const node, int lvl);
    void do_div_node(cdk::div_node * const node, int lvl);
    void do_mod_node(cdk::mod_node * const node, int lvl);
    void do_lt_node(cdk::lt_node * const node, int lvl);
    void do_le_node(cdk::le_node * const node, int lvl);
    void do_ge_node(cdk::ge_node * const node, int lvl);
    void do_gt_node(cdk::gt_node * const node, int lvl);
    void do_ne_node(cdk::ne_node * const node, int lvl);
    void do_eq_node(cdk::eq_node * const node, int lvl);
    void do_or_node(pwn::or_node * const node, int lvl);
    void do_and_node(pwn::and_node * const node, int lvl);
    void do_index_node(pwn::index_node * const node, int lvl);
    

  public:
    void do_repeat_node(pwn::repeat_node * const node, int lvl);
    void do_while_node(cdk::while_node * const node, int lvl);
    void do_if_node(cdk::if_node * const node, int lvl);
    void do_if_else_node(cdk::if_else_node * const node, int lvl);
    void do_noob_node(pwn::noob_node * const node, int lvl);
    void do_next_node(pwn::next_node * const node, int lvl);
    void do_return_node(pwn::return_node * const node, int lvl);
    void do_stop_node(pwn::stop_node * const node, int lvl);
    
  public:
    void do_rvalue_node(pwn::rvalue_node * const node, int lvl);

  public:
    void do_evaluation_node(pwn::evaluation_node * const node, int lvl);
    void do_print_node(pwn::print_node * const node, int lvl);
    void do_println_node(pwn::println_node * const node, int lvl);
    void do_read_node(pwn::read_node * const node, int lvl);
    void do_assignment_node(pwn::assignment_node * const node, int lvl);
    void do_addressing_node(pwn::addressing_node * const node , int lvl);
    void do_decl_var_node(pwn::decl_var_node * const node , int lvl);
    void do_variable_node(pwn::variable_node * const node , int lvl);
    void do_decl_func_node(pwn::decl_func_node * const node , int lvl);
    void do_call_func_node(pwn::call_func_node * const node , int lvl);
    void do_def_func_node(pwn::def_func_node * const node , int lvl);
    void do_block_node(pwn::block_node * const node , int lvl);
    void do_apply_from_to_node(pwn::apply_from_to_node * const node , int lvl){}
  };

} // pwn

#endif
