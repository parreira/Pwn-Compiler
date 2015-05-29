#ifndef __PWN_ORNODE_H__
#define __PWN_ORNODE_H__

#include <cdk/ast/binary_expression_node.h>

namespace pwn {

  /**
   * Class for describing print nodes.
   */
  class or_node: public cdk::binary_expression_node {

  public:
    inline or_node(int lineno, cdk::expression_node *left, cdk::expression_node *right) :
        cdk::binary_expression_node(lineno, left, right){
    }

  public:
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_or_node(this, level);
    }

  };

} // pwn

#endif
  
