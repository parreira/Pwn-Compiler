// $Id: postfix_writer.h,v 1.43 2015/05/20 06:44:13 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_PF_WRITER_H__
#define __PWN_SEMANTICS_PF_WRITER_H__


#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/integer_node.h>
#include <cdk/ast/binary_expression_node.h>
#include <cdk/ast/unary_expression_node.h>
#include <cdk/ast/simple_value_node.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace pwn {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<pwn::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    
    int _lbl;
    
    std::vector<std::string> _stop_labels; 
    std::vector<std::string> _next_labels;
    std::vector<std::string> _imports;
    std::vector<std::string> _undefSymbol; //the undefined symbols that we may import..
    
    bool _global = true; //global context ?
    bool _indexing = false; //used for global constant string.. or not
    
    int local_arg_offset ; //OFFSET OF ARGUMENTS
    int local_decl_offset ; //OFFSET OF DECLARATION
    
    pwn::decl_func_node * this_function  ; //THE ACTUAL FUNCTION
    
  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<pwn::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0), _global(true) {
    }
    

  public:
    ~postfix_writer() {
      os().flush();
    }
    
    enum Op {
      add ,
      sub ,
      mul ,
      div ,
      neg ,
      mod ,
      lt  ,
      le  ,
      ge  ,
      gt ,
      ne ,
      eq 
    };
    
    enum Segment {
      bss,
      data,
      rodata,
      text
    };
    
    Segment oldSegment ;
    Segment actualSegment ; 
    
    inline void assembleSegment(){
      switch(actualSegment){
	case Segment::bss : _pf.BSS(); return;
	case Segment::data: _pf.DATA(); return;
	case Segment::rodata: _pf.RODATA(); return;
	case Segment::text : _pf.TEXT(); return;
      }
    }
    
    inline void setSegm(Segment s){ 
      if(actualSegment != s){
	oldSegment = actualSegment;
	actualSegment = s;
	assembleSegment();
	_pf.ALIGN();
      }
      else{
	oldSegment = actualSegment;
	actualSegment = s;
      }

    }
    
    inline void setOldSegmn(){
      actualSegment = oldSegment;
      assembleSegment();
      _pf.ALIGN();
    }

    inline void assembleGlobalConstantVariable(int value){
      int lbl1 = ++_lbl;
      setSegm(Segment::rodata); 
      _pf.LABEL(mklbl(lbl1)); 
      _pf.CONST(value);
      setOldSegmn(); //continue to do whatever i was doing before
      //AUTO LOAD IT TO THE STACK!
      _pf.ADDR(mklbl(lbl1));
      _pf.LOAD();
    }
    inline void assembleGlobalNonConstantVariable(int value){
      _pf.CONST(value);
    }
    inline void assembleLocalNonConstantVariable(int value){
      _pf.INT(value);
    }
    inline void assembleLocalConstantVariable(int value){
      _pf.INT(value);
    }
    inline void assembleGlobalConstantVariable(double value){
          int lbl1 = ++_lbl;
	  setSegm(Segment::rodata); //rodata  	  
	  _pf.LABEL(mklbl(lbl1));
	  _pf.DOUBLE(value);
    }
    inline void assembleGlobalNonConstantVariable(double value){
      _pf.DOUBLE(value);
    }
    inline void assembleLocalNonConstantVariable(double value){
          int lbl1 = ++_lbl;
	  setSegm(Segment::rodata); //rodata  
	  
	  _pf.LABEL(mklbl(lbl1));
	  _pf.DOUBLE(value);
	  setOldSegmn(); //continue to do whatever i was doing before
	  //AUTO LOAD IT TO THE STACK!
	  _pf.ADDR(mklbl(lbl1));
	  _pf.DLOAD();
	  
    }
    inline void assembleLocalConstantVariable(double value){
          int lbl1 = ++_lbl;
	  setSegm(Segment::rodata); //rodata  
	  
	  _pf.LABEL(mklbl(lbl1));
	  _pf.DOUBLE(value);
	  setOldSegmn(); //continue to do whatever i was doing before
	  //AUTO LOAD IT TO THE STACK!
	  _pf.ADDR(mklbl(lbl1));
	  _pf.DLOAD();
    }
    
    inline void assembleGlobalNonConstantVariable(std::string value){
      int lbl1 = ++_lbl;
      setSegm(Segment::rodata); //rodata
      _pf.LABEL(mklbl(lbl1));
      _pf.STR(value); 
       setOldSegmn();
      _pf.ID(mklbl(lbl1));
    }
    inline void assembleLocalNonConstantVariable(std::string value){
      int lbl1 = ++_lbl;
      setSegm(Segment::rodata); //rodata
      _pf.LABEL(mklbl(lbl1)); 
      _pf.STR(value);
      setOldSegmn();
      //AUTO LOAD IT TO THE STACK!
      _pf.ADDR(mklbl(lbl1));
    }
    inline void assembleLocalConstantVariable(std::string value){
      assembleLocalNonConstantVariable(value);
    }
    
    inline void assembleGlobalConstantVariable(std::string value){
	int lbl1 = ++_lbl;
	setSegm(Segment::rodata); //rodata
	 // make sure we are aligned
	_pf.LABEL(mklbl(lbl1)); // give the string a name
	_pf.STR(value); // output string characters
    }
    
    template<typename typeName>
    inline void declareVariable(typeName values){
      bool constant = (actualSegment == rodata);
      if(_global && constant){ //global e constante
	assembleGlobalConstantVariable(values);
      }
      else{
	if(_global && !constant){ //global e nao constante
	  assembleGlobalNonConstantVariable(values);
	}
	else{
	  if(!_global && constant){//local e constante
	    assembleLocalConstantVariable(values);
	  }
	  else{ // local e nao constante
	    assembleLocalNonConstantVariable(values);
	  }
	}
      }
    }
    
    inline void insertUndefSymbol(std::string s){
      for(auto &symbol : _undefSymbol){
	if(symbol == s){
	  return;
	}
      }
      _undefSymbol.push_back(s);
    }
    
    inline void insertImport(std::string newImport){
      for(auto &import : _imports){
	if(import == newImport){
	  return;
	}
      }
      _imports.push_back(newImport);
      
    }
    inline void externUnDefSymbols(){
      std::shared_ptr<pwn::symbol> symbol ;
      for(auto &name : _undefSymbol){
	symbol = _symtab.find(name);
	if(!symbol->defined()){
	  _pf.EXTERN(name);
	}
      }
    }
    
    inline void externImports(){
      for(auto &import : _imports){
	_pf.EXTERN(import);
      }
    }
    
    inline void setLocalOffSets(){
      local_arg_offset = +8;
      local_decl_offset = 0;
    }
    inline void convertDoubleInt(Op operation,basic_type * resultType,
      cdk::expression_node * node, int lvl){
	node->accept(this, lvl +1);
	changeDoubleInt(node->type(),resultType);
	doOperation(resultType,operation);
      }
    
    inline void convertDoubleInt(Op operation,basic_type * resultType,
	cdk::expression_node * l,cdk::expression_node * r,int lvl){
      l->accept(this, lvl +1);
      changeDoubleInt(l->type(),resultType);
      r->accept(this, lvl +1);
      changeDoubleInt(r->type(),resultType);
      doOperation(resultType,operation);
    }
    
    inline void changeDoubleInt(basic_type * node, basic_type * result){
      if(node->name() == basic_type::TYPE_INT 
	&& result->name() == basic_type::TYPE_DOUBLE ){ 
	_pf.I2D();
      }
      else{
	if(node->name() == basic_type::TYPE_DOUBLE
	  && result->name() == basic_type::TYPE_INT ){ 
	  _pf.D2I();
	}
      }
    }
    
    
    inline void doOperation(basic_type * resultType, postfix_writer::Op o){
      if(resultType->name() == basic_type::TYPE_DOUBLE){
	switch(o){
	  case Op::add: _pf.DADD(); return;
	  case Op::sub:	_pf.DSUB(); return;
	  case Op::mul: _pf.DMUL(); return;
	  case Op::div:	_pf.DDIV(); return;
	  case Op::neg:	_pf.DNEG(); return;
	  case Op::mod: _pf.D2I(); _pf.SWAP(); _pf.D2I(); _pf.MOD(); return;
	  //this must be resultTYPE-> INT
	  case Op::lt: _pf.DCMP(); _pf.INT(0);_pf.LT();  return;
	  case Op::le: _pf.DCMP(); _pf.INT(0);_pf.LE();  return;
	  case Op::ge: _pf.DCMP(); _pf.INT(0);_pf.GE();  return;
	  case Op::gt: _pf.DCMP(); _pf.INT(0);_pf.GT();  return;
	  case Op::ne: _pf.DCMP(); _pf.INT(0); _pf.NE(); return;
	  case Op::eq: _pf.DCMP(); _pf.INT(0); _pf.EQ(); return;
	}
      }
      else{
	switch(o){
	  case Op::add: _pf.ADD(); return;
	  case Op::sub:	_pf.SUB(); return;
	  case Op::mul: _pf.MUL(); return;
	  case Op::div:	_pf.DIV(); return;
	  case Op::neg:	_pf.NEG(); return;
	  case Op::mod: _pf.MOD(); return;
	  case Op::lt:  _pf.LT(); return;
	  case Op::le:  _pf.LE(); return;
	  case Op::ge:  _pf.GE(); return;
	  case Op::gt:  _pf.GT(); return;
	  case Op::ne: 	_pf.NE(); return;
	  case Op::eq:  _pf.EQ(); return;
	}
      }
    }
  inline void doFuncReturn(basic_type * s){
    if(s->name() != basic_type::TYPE_VOID){
      if(s->name() == basic_type::TYPE_DOUBLE){  
	_pf.TRASH(8);
	_pf.DPUSH();
      }
      else{
	_pf.TRASH(4);
	_pf.PUSH();
      }
    }
  }  
  inline void popFuncReturn(basic_type * s){
    if(s->name() != basic_type::TYPE_VOID){
      _pf.LOCAL(0 - s->size());
      if(s->name() == basic_type::TYPE_DOUBLE){
	_pf.DLOAD();
	_pf.DPOP();
      }
      else{	
	_pf.LOAD();
	_pf.POP();
      }
    } 
    _pf.LEAVE();
    _pf.RET();
  }
  
  inline void saveReturnVariable(basic_type * s, cdk::expression_node * literal, int lvl){
    local_decl_offset -= s->size();
    if(literal != NULL){ // is there a default return ?
      literal->accept(this, lvl +1);
      doDup(s);
      _pf.LOCAL(0 - s->size());
      if(s->name() == basic_type::TYPE_DOUBLE){
	_pf.DSTORE();
      }
      else{
	_pf.STORE();
      }
    }    
  }
  inline void doDup(basic_type * s){   
    if(s->name() == basic_type::TYPE_DOUBLE){
      _pf.DDUP();
    }
    else{
      _pf.DUP();
    }
  } 
  inline void doStore(basic_type * s){
    if(s->name() == basic_type::TYPE_DOUBLE){
      _pf.DSTORE();
    }
    else{
      _pf.STORE();
    }
  }
  inline void doLoad(basic_type * s){
    if(s->name() == basic_type::TYPE_DOUBLE){
      _pf.DLOAD();
    }
    else{
      _pf.LOAD();
    }    
  }
  
  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }
    
   

  public:
    void do_sequence_node(cdk::sequence_node * const node, int lvl);

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);
    void do_double_node(cdk::double_node * const node, int lvl);
  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);
    void do_not_node(pwn::not_node * const node, int lvl);
    void do_identity_node(pwn::identity_node * const node, int lvl);
    void do_memory_reservation_node(pwn::memory_reservation_node * const node, int lvl);

  public:
    void do_add_node(cdk::add_node * const node, int lvl);
    void do_sub_node(cdk::sub_node * const node, int lvl);
    void do_mul_node(cdk::mul_node * const node, int lvl);
    void do_div_node(cdk::div_node * const node, int lvl);
    void do_mod_node(cdk::mod_node * const node, int lvl);
    void do_lt_node(cdk::lt_node * const node, int lvl);
    void do_le_node(cdk::le_node * const node, int lvl);
    void do_ge_node(cdk::ge_node * const node, int lvl);
    void do_gt_node(cdk::gt_node * const node, int lvl);
    void do_ne_node(cdk::ne_node * const node, int lvl);
    void do_eq_node(cdk::eq_node * const node, int lvl);
    void do_or_node(pwn::or_node * const node, int lvl);
    void do_and_node(pwn::and_node * const node, int lvl);
    void do_index_node(pwn::index_node * const node, int lvl);

  public:
    void do_repeat_node(pwn::repeat_node * const node, int lvl);
    void do_while_node(cdk::while_node * const node, int lvl){}
    void do_if_node(cdk::if_node * const node, int lvl);
    void do_if_else_node(cdk::if_else_node * const node, int lvl);
    void do_noob_node(pwn::noob_node * const node, int lvl);
    void do_next_node(pwn::next_node * const node, int lvl);
    void do_return_node(pwn::return_node * const node, int lvl);
    void do_stop_node(pwn::stop_node * const node, int lvl);

  public:
    void do_rvalue_node(pwn::rvalue_node * const node, int lvl);

  public:
    void do_evaluation_node(pwn::evaluation_node * const node, int lvl);
    void do_print_node(pwn::print_node * const node, int lvl);
    void do_println_node(pwn::println_node * const node, int lvl);
    void do_read_node(pwn::read_node * const node, int lvl);
    void do_assignment_node(pwn::assignment_node * const node, int lvl);
    void do_addressing_node(pwn::addressing_node * const node , int lvl);
    void do_decl_var_node(pwn::decl_var_node * const node , int lvl);
    void do_variable_node(pwn::variable_node * const node , int lvl);
    void do_decl_func_node(pwn::decl_func_node * const node , int lvl);
    void do_call_func_node(pwn::call_func_node * const node , int lvl);
    void do_def_func_node(pwn::def_func_node * const node , int lvl);
    void do_block_node(pwn::block_node * const node , int lvl);
    void do_apply_from_to_node(pwn::apply_from_to_node * const node , int lvl);
  };

} // pwn

#endif
