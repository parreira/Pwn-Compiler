 
#ifndef __PWN_MEMORYRESERVATIONNODE_H__
#define __PWN_MEMORYRESERVATIONNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace pwn {

  /**
   * Class for describing print nodes.
   */
  class memory_reservation_node: public cdk::unary_expression_node {
  public:
    inline memory_reservation_node(int lineno, cdk::expression_node *argument) :
        unary_expression_node(lineno,argument){
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_reservation_node(this, level);
    }

  };

} // pwn

#endif
 