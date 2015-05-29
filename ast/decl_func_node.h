// $Id: decl_func_node.h,v 1.8 2015/04/27 02:56:22 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_DECLFUNCNODE_H__
#define __PWN_DECLFUNCNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>
#include <cdk/ast/basic_node.h>

namespace pwn {

  /**
   * Class for describing noob nodes.
   */
  class decl_func_node: public cdk::basic_node {
    basic_type * _type;
    cdk::expression_node *_literal;
    std::string _lvalue;
    bool _local;
    bool _import ;
    cdk::sequence_node * _arguments;
    

  public:
    inline decl_func_node(int lineno,basic_type * type, cdk::expression_node * literal,
			std::string* lvalue,bool local, bool import, cdk::sequence_node * arguments) :
	cdk::basic_node(lineno), _type(type), _literal(literal), _lvalue(*lvalue),_local(local),_import(import), _arguments(arguments){ }

  public:
    inline basic_type * type(){
      return _type;
    }
    inline cdk::expression_node * literal(){
      return _literal;
    }
    inline void type(basic_type * type){
      _type = type;
    }
    
    inline std::string lvalue(){
      return _lvalue;
    }
    inline bool isLocal(){
      return _local;
    }
    inline bool isImport(){
      return _import;
    }
    
    inline cdk::sequence_node * arguments(){
      return _arguments;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_decl_func_node(this, level);
    }

  };

} // pwn

#endif
