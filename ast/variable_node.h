// $Id: variable_node.h,v 1.14 2015/05/18 17:36:37 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_VARNODE_H__
#define __PWN_VARNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>
#include <ast/lvalue_node.h>
#include <ast/decl_var_node.h>

namespace pwn {

  /**
   * Class for describing noob nodes.
   */
  class variable_node: public pwn::lvalue_node{
    bool _global;
    
  public:
    inline variable_node(int lineno, std::string * identifier, bool global = false) :
	pwn::lvalue_node(lineno,identifier), _global(global){ }

  public:
    inline std::string value(){
      return _value;
    }
    
    inline bool isGlobal(){
      return _global;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_node(this, level);
    }

  };

} // pwn

#endif
