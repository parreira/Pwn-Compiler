// $Id: addressing_node.h,v 1.3 2015/03/23 19:31:06 ist175814 Exp $
#ifndef __PWN_ADDRESSINGVALUE_NODE_H__
#define __PWN_ADDRESSINGVALUE_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>
namespace pwn {

  /**
   * Class for describing address nodes.
   */
  class addressing_node: public cdk::expression_node{
      pwn::lvalue_node * _lvalue;
    
    
  public:
    inline addressing_node(int lineno, pwn::lvalue_node *lvalue ) :
        cdk::expression_node(lineno), _lvalue(lvalue) {  
    }
    
    inline pwn::lvalue_node * argument() {
        return _lvalue;
    }
    

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_addressing_node(this, level);
    }

  };

} // pwn

#endif
