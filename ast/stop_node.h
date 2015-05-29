// $Id: stop_node.h,v 1.3 2015/04/27 02:56:22 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_STOPNODE_H__
#define __PWN_STOPNODE_H__


namespace pwn {

  /**
   * Class for describing read nodes.
   */
  class stop_node : public cdk::basic_node {
    cdk::integer_node *_argument;

  public:
    inline stop_node(int lineno, cdk::integer_node *argument ) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::integer_node *argument() {
      return _argument;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }

  };

} // pwn

#endif

