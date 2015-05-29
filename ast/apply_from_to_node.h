// $Id: apply_from_to_node.h,v 1.2 2015/04/07 22:10:05 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_APPLYFROMTONODE_H__
#define __PWN_APPLYFROMTONODE_H__

#include "ast/lvalue_node.h"

namespace pwn {

  /**
   * Class for describing assignment nodes.
   */
  class apply_from_to_node: public cdk::basic_node {
    std::string _function;
    pwn::lvalue_node * _lval ;
    cdk::expression_node *_lvalue;
    cdk::expression_node *_rvalue;

  public:
    inline apply_from_to_node(int lineno,std::string * function,pwn::lvalue_node * lval ,cdk::expression_node * lvalue, cdk::expression_node *rvalue) :
        cdk::basic_node(lineno),_function(*function), _lval(lval) ,_lvalue(lvalue), _rvalue(rvalue) {
    }

  public:
    inline std::string function(){
      return _function;
    }
    
    inline pwn::lvalue_node * lval(){
      return _lval;
    }
    inline cdk::expression_node *lvalue() {
      return _lvalue;
    }
    inline cdk::expression_node *rvalue() {
      return _rvalue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_apply_from_to_node(this, level);
    }

  };

} // pwn

#endif
