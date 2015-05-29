// $Id: noob_node.h,v 1.2 2015/03/23 19:31:06 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_NOOBNODE_H__
#define __PWN_NOOBNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing noob nodes.
   */
  class noob_node: public cdk::expression_node {
    cdk::expression_node *_argument;

  public:
    inline noob_node(int lineno) :
        cdk::expression_node(lineno) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_noob_node(this, level);
    }

  };

} // pwn

#endif
