// $Id: assignment_node.h,v 1.2 2015/04/07 22:10:05 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_ASSIGNMENTNODE_H__
#define __PWN_ASSIGNMENTNODE_H__

#include "ast/lvalue_node.h"

namespace pwn {

  /**
   * Class for describing assignment nodes.
   */
  class assignment_node: public cdk::expression_node {
    pwn::lvalue_node *_lvalue;
    cdk::expression_node *_rvalue;

  public:
    inline assignment_node(int lineno, pwn::lvalue_node * lvalue, cdk::expression_node *rvalue) :
        cdk::expression_node(lineno), _lvalue(lvalue), _rvalue(rvalue) {
    }

  public:
    inline pwn::lvalue_node *lvalue() {
      return _lvalue;
    }
    inline cdk::expression_node *rvalue() {
      return _rvalue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_assignment_node(this, level);
    }

  };

} // pwn

#endif
