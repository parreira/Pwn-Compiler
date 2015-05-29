#ifndef __PWN_INDEXNODE_H__
#define __PWN_INDEXNODE_H__


namespace pwn {

  /**
   * Class for describing print nodes.
   */
  class index_node: public pwn::lvalue_node {
    cdk::expression_node * _lval; 
    cdk::expression_node * _rval; 

  public:
    inline index_node(int lineno, cdk::expression_node * lval, cdk::expression_node *right) :
        pwn::lvalue_node(lineno),_lval(lval), _rval(right)
	{ }

  public:
    
    inline cdk::expression_node * lval(){
      return _lval;
    }
    
    inline cdk::expression_node * rval(){
      return _rval;
    }
    
    inline std::string value(){
      return _value;
    }
    
    inline void value(std::string value){
      _value = value;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // pwn

#endif
  
