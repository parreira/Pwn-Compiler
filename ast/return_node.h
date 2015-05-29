// $Id: return_node.h,v 1.2 2015/03/12 13:12:13 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_RETURNNODE_H__
#define __PWN_RETURNNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing return nodes.
   */
  class return_node: public cdk::expression_node {
    
  public:
    inline return_node(int lineno): cdk::expression_node(lineno) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // pwn

#endif
