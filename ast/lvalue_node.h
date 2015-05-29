// $Id: lvalue_node.h,v 1.14 2015/05/18 17:36:37 ist175814 Exp $
#ifndef __PWN_NODE_EXPRESSION_LEFTVALUE_H__
#define __PWN_NODE_EXPRESSION_LEFTVALUE_H__
#include <string>

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding lvalues.
   */
  class lvalue_node: public cdk::expression_node{
  public:
    std::string _value ; 
    
    inline lvalue_node(int lineno) :
        cdk::expression_node(lineno){
    }
    inline lvalue_node(int lineno, std::string * value) :
        cdk::expression_node(lineno), _value(*value){
    }
   
    virtual std::string value() = 0;
    
  };
} // pwn
#endif