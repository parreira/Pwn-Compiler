// $Id: repeat_node.h,v 1.3 2015/04/07 22:10:05 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_REPEATNODE_H__
#define __PWN_REPEATNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing while-cycle nodes.
   */
  class repeat_node: public cdk::basic_node {
    cdk::sequence_node *_init;
    cdk::sequence_node *_condition;
    cdk::sequence_node *_increment;
    cdk::basic_node *_block;

  public:
    inline repeat_node(int lineno, cdk::sequence_node *init,
	cdk::sequence_node *condition,cdk::sequence_node *increment, cdk::basic_node *block) :
        cdk::basic_node(lineno), _init(init), _condition(condition)
	, _increment(increment), _block(block) {
    }

  public:
    inline cdk::sequence_node *init() {
      return _init;
    }
    inline cdk::sequence_node *condition() {
      return _condition;
    }
    inline cdk::sequence_node *increment() {
      return _increment;
    }
    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_repeat_node(this, level);
    }

  };

} // pwn

#endif