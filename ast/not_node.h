#ifndef __PWN_NOTNODE_H__
#define __PWN_NOTNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace pwn {

  /**
   * Class for describing print nodes.
   */
  class not_node: public cdk::unary_expression_node {

  public:
    inline not_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument){
    }

  public:
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_not_node(this, level);
    }

  };

} // pwn

#endif
 
