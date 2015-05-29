// $Id: block_node.h,v 1.2 2015/04/07 22:10:05 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_BLOCKCNODE_H__
#define __PWN_BLOCKNODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/basic_node.h>

namespace pwn {

  /**
   * Class for describing noob nodes.
   */
  class block_node: public cdk::basic_node {
    cdk::sequence_node * _stmts;
    cdk::sequence_node * _instrs;
    
  public:
    inline block_node(int lineno, cdk::sequence_node * stmts,cdk::sequence_node * instrs) :
	cdk::basic_node(lineno), _stmts(stmts), _instrs(instrs){ }

  public:
    
    inline cdk::sequence_node * stmts(){
      return _stmts;
    }
    
    inline cdk::sequence_node * instrs(){
      return _instrs;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // pwn

#endif
