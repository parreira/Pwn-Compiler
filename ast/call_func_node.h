// $Id: call_func_node.h,v 1.5 2015/04/30 01:20:57 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_CALLFUNCNODE_H__
#define __PWN_CALLFUNCNODE_H__

#include <cdk/ast/sequence_node.h>

namespace pwn {

  /**
   * Class for describing noob nodes.
   */
  class call_func_node: public cdk::expression_node {
    std::string _identifier;
    cdk::sequence_node * _arguments;
    
  public:
    inline call_func_node(int lineno,std::string * identifier, cdk::sequence_node * arguments) :
	cdk::expression_node(lineno), _identifier(*identifier), _arguments(arguments){ }

  public:
    inline std::string identifier(){
      return _identifier;
    }
    
    inline cdk::sequence_node * arguments(){
      return _arguments;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_call_func_node(this, level);
    }

  };

} // pwn

#endif
