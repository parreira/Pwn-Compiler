// $Id: decl_var_node.h,v 1.8 2015/05/03 19:51:53 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_DECLVARNODE_H__
#define __PWN_DECLVARNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>
#include <ast/lvalue_node.h>
#include <ast/variable_node.h>
namespace pwn {

  /**
   * Class for describing noob nodes.
   */
  class decl_var_node: public pwn::variable_node {
    bool _local;
    bool _import ;
    bool _func_argument ;
    bool _isConst;
    
  public:
    inline decl_var_node(
	int lineno, basic_type * type, std::string * identifier,bool global,
	  bool local, bool import, bool func_argument,bool isConst
			) :
	pwn::variable_node(lineno,identifier,global)
	  ,_local(local),_import(import),_func_argument(func_argument),_isConst(isConst){ 
	  _type=type;
	  }

  public:
    inline bool isLocal(){
      return _local;
    }
    inline bool isImport(){
      return _import;
    }
    inline bool isFuncArgument(){
      return _func_argument;
    }
    
    inline bool isConst(){
      return _isConst;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_decl_var_node(this, level);
    }

  };

} // pwn

#endif
