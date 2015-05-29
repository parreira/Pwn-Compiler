// $Id: symbol.h,v 1.15 2015/05/06 01:17:11 ist175814 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_SYMBOL_H__
#define __PWN_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace pwn {

    class symbol {
      basic_type *_type;
      bool _defined ;
      bool _local = false;
      bool _import = false;
      bool _isConst;

      std::string _label; // 4 global constant symbols
      int _offset; // 4 local symbols
      basic_type** _argsTypes;
      
      bool _assigning = false;
      
    public:
      enum Context {
	function,
	global_var,
	local_var
      };
      Context _c;
      
      inline void setContext(symbol::Context c){
	_c = c;
      }
      
      //LOCAL VARIABLES !!!
      inline symbol(basic_type *type, bool defined, int offset) :
          _type(type), _defined(defined) , _offset(offset),_c(Context::local_var){  
      } 
      
      //GLOBAL VARIABLES !!!
      inline symbol(basic_type * type, bool defined,bool local, bool import, bool isConst, std::string  label):
	_type(type), _defined(defined),_local(local),_import(import), _isConst(isConst), _label(label), _c(Context::global_var){
	  
	}
      //FUNCTIONS !!!
      inline symbol(basic_type * type, bool defined,bool local ,bool import,int offset ,basic_type ** argsTypes):
	_type(type), _defined(defined), _local(local),_import(import),_offset(offset) ,_argsTypes(argsTypes), _c(Context::function){  
	}
	
      inline bool isFunction() {
        return _c == Context::function;
      } 
      inline bool isGlobalVar() {
        return _c == Context::global_var;
      }
      inline bool isLocalVar() {
        return _c == Context::local_var;
      }

      virtual ~symbol() {
      }

      inline basic_type *type() {
        return _type;
      }
      
      inline bool defined() {
        return _defined;
      }
      inline void define(bool d) {
	 _defined = d;
      }
      
      inline bool isConst(){
	return _isConst;
      }
      
      inline void label(std::string label){
	_label=label;
      }
      
      inline std::string label(){
	return _label;
      }
      
      inline void offset(int offset){
	_offset=offset;
      }

      inline int offset(){
	return _offset;
      }
      inline void argsType(basic_type** args){
	_argsTypes = args;
      }
      
      inline basic_type** getArgsTypes(){
	return _argsTypes;
      }
      
      inline void assigning(bool assign){
	_assigning = assign;
      }
      inline bool assigning(){
	return _assigning;
      }
      
      inline bool isLocal(){
	return _local;
      }
      inline bool isImport(){
	return _import;
      }
      
    };

} // pwn

#endif
