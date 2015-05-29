// $Id: def_func_node.h,v 1.2 2015/04/07 22:10:05 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_DEFFUNCNODE_H__
#define __PWN_DEFFUNCNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>
#include <cdk/ast/basic_node.h>

namespace pwn {

  /**
   * Class for describing noob nodes.
   */
  class def_func_node: public cdk::basic_node {
    pwn::decl_func_node * _func_decl ;
    cdk::basic_node * _block;
    
  public:
    inline def_func_node(int lineno,pwn::decl_func_node * func_decl,cdk::basic_node * block) :
	cdk::basic_node(lineno), _func_decl(func_decl), _block(block){ }

  public:
    inline pwn::decl_func_node * func_decl(){
      return _func_decl;
    }
    
    inline cdk::basic_node * block(){
      return _block;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_def_func_node(this, level);
    }

  };

} // pwn

#endif
