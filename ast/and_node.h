#ifndef __PWN_ANDNODE_H__
#define __PWN_ANDNODE_H__

#include <cdk/ast/binary_expression_node.h>

namespace pwn {

  /**
   * Class for describing print nodes.
   */
  class and_node: public cdk::binary_expression_node {

  public:
    inline and_node(int lineno, cdk::expression_node *left, cdk::expression_node *right) :
        cdk::binary_expression_node(lineno, left, right){
    }

  public:
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_and_node(this, level);
    }

  };

} // pwn

#endif
  
 
