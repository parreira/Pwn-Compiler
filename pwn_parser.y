%{
// $Id: pwn_parser.y,v 1.38 2015/05/19 18:18:24 ist175814 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;	/* integer value */
  std::string          *s;	/* symbol name or string literal */
  double 		d;
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  basic_type		*type;
  pwn::lvalue_node  	*lvalue;
  pwn::decl_func_node * funcdecl;
};

/* LITERAIS */
%token <i> tINTEGER 
%token <d> tREAL
%token <s> tIDENTIFIER tSTRING

/* TOKENS */
%token tREPEAT tIF tPRINTLN tBEGIN tEND tLOCAL tIMPORT tNEXT tSTOP tRETURN tNOOB tINTEGER tREAL tIDENTIFIER tSTRING tAPPLY tFROM tTO
%nonassoc tIFX
%nonassoc tELSE

%right '=' 
%left '|' 
%left '&'
%nonassoc '~'
%left tEQ tNE 
%left '<' '>' tGE tLE   
%left '+' '-' 
%left '*' '/' '%'
%nonassoc tIDENTITY tNEG '?'
%nonassoc '(' '['  '@'

%type <node> arg decl varstmt instr block cond cicle cycle_instr assignvar
%type <sequence> decls vars args stmts instrs exprs exprs_args
%type <expression> literal expr funcall variable_expr
%type <lvalue> lval variable declvar external_var
%type <type> type
%type <funcdecl> external_func func
%type <s> string 

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%


program : decls { compiler->ast($1);}
	  |	{ compiler->ast(new cdk::nil_node(LINE));} //well the clean file ...
	  ;
	 
decls : decl 		{$$ = new cdk::sequence_node(LINE,$1);}
	| decls decl 	{$$ = new cdk::sequence_node(LINE,$2,$1);}
	;

decl : assignvar ';'	{$$ = $1;}
      | external_func 	{$$ = $1;}
      | external_var	{$$ = $1;}
      | func block      {$$ = new pwn::def_func_node(LINE,$1,$2);}
      ;

declvar :  tLOCAL type tIDENTIFIER  		{$$ = new pwn::decl_var_node(LINE,$2,$3,true,true,false,false,false);}
	| type tIDENTIFIER			{$$ = new pwn::decl_var_node(LINE,$1,$2,true,false,false,false,false);}
	| tLOCAL '<' type '>' tIDENTIFIER  	{$$ = new pwn::decl_var_node(LINE,$3,$5,true,true,false,false,true);}
	| '<' type '>' tIDENTIFIER		{$$ = new pwn::decl_var_node(LINE,$2,$4,true,false,false,false,true);}
	;

	
assignvar: declvar 				{$$ = $1; }
	  | declvar '=' expr 			{$$ = new pwn::assignment_node(LINE,$1,$3);}
	  ;


type : 	 '#' 		{$$= new basic_type(4,basic_type::TYPE_INT);}
	| '%' 		{$$= new basic_type(8,basic_type::TYPE_DOUBLE);}
	| '$' 		{$$= new basic_type(4,basic_type::TYPE_STRING);}
	| '*'		{$$= new basic_type(4,basic_type::TYPE_POINTER);}
	;
	
external_var:  tIMPORT '<' type '>' tIDENTIFIER  	{$$ = new pwn::decl_var_node(LINE,$3,$5,true,false,true,false,true);}
	      | tIMPORT type tIDENTIFIER  		{$$ = new pwn::decl_var_node(LINE,$2,$3,true,false,true,false,false);}
	      ;

external_func:  tIMPORT type tIDENTIFIER '(' vars ')' {$$ = new pwn::decl_func_node(LINE,$2,NULL,$3,false,true,$5);}
	       | tIMPORT '!' tIDENTIFIER '(' vars ')' {$$ = new pwn::decl_func_node(LINE,new basic_type(0,basic_type::TYPE_VOID),NULL,$3,false,true,$5);}
	       | func {$$ = $1;}
	      ;

vars :	 args	{$$ = $1;}
      | 	{$$ = NULL;}
      ;
      
args:	arg 		{$$ = new cdk::sequence_node(LINE,$1);}
      | args ',' arg 	{$$ = new cdk::sequence_node(LINE,$3,$1);}
      ;

arg : type tIDENTIFIER {$$ = new pwn::decl_var_node(LINE,$1,$2,false,false,false,true,false);}
    ;
func :	tLOCAL type tIDENTIFIER '(' vars ')' '=' literal  {$$ = new pwn::decl_func_node(LINE,$2,$8,$3,true,false,$5);}	   
	| tLOCAL '!' tIDENTIFIER '(' vars ')'   {$$ = new pwn::decl_func_node(LINE,new basic_type(0,basic_type::TYPE_VOID),NULL,$3,true,false,$5);}
	| tLOCAL type tIDENTIFIER '(' vars ')'   {$$ = new pwn::decl_func_node(LINE,$2,NULL,$3,true,false,$5);}
	| type tIDENTIFIER '(' vars ')'  {$$ = new pwn::decl_func_node(LINE,$1,NULL,$2,false,false,$4);}
	| type tIDENTIFIER '(' vars ')' '=' literal  {$$ = new pwn::decl_func_node(LINE,$1,$7,$2,false,false,$4);}	
	| '!' tIDENTIFIER '(' vars ')'  {$$ = new pwn::decl_func_node(LINE,new basic_type(0,basic_type::TYPE_VOID),NULL,$2,false,false,$4);}
	;

block :	'{' stmts instrs '}'		{$$ = new pwn::block_node(LINE,$2,$3);}
      | '{' stmts '}'			{$$ = new pwn::block_node(LINE,$2,NULL);}
      | '{' instrs '}'			{$$ = new pwn::block_node(LINE,NULL,$2);}
      | '{' '}'				{$$ = new pwn::block_node(LINE,NULL,NULL);}
      ;
      
stmts :	 varstmt 		{$$ = new cdk::sequence_node(LINE,$1);}
	| stmts varstmt 	{$$ = new cdk::sequence_node(LINE,$2,$1);}
	;	
	

variable: type tIDENTIFIER		{$$ = new pwn::decl_var_node(LINE,$1,$2,false,false,false,false,false);}
	| '<' type '>' tIDENTIFIER	{$$ = new pwn::decl_var_node(LINE,$2,$4,false,false,false,false,true);}
	;
	
varstmt : variable ';'	    {$$ = $1;} 
	| variable '=' expr ';' {$$ = new pwn::assignment_node(LINE, $1, $3);}
	;

instrs:  instr 		{$$ = new cdk::sequence_node(LINE,$1);}
	| instrs instr 	{$$ = new cdk::sequence_node(LINE,$2,$1);}
	;

instr: 	  expr ';'		{$$= new pwn::evaluation_node(LINE,$1);}
	| expr '!'		{$$= new pwn::print_node(LINE,$1);}
	| expr tPRINTLN		{$$= new pwn::println_node(LINE,$1);} 
	| cycle_instr ';'	{$$ = $1;}
	| cond			{$$ = $1;}
	| cicle			{$$ = $1;}
	| block			{$$ = $1;}
	| tRETURN		{$$= new pwn::return_node(LINE);}
	| tAPPLY tIDENTIFIER tTO lval tFROM expr tIDENTIFIER expr ';' {$$ = new pwn::apply_from_to_node(LINE,$2,$4,$6,$8);}
	;

cycle_instr:  tSTOP 		{$$= new pwn::stop_node(LINE,new cdk::integer_node(LINE,1));}
	    | tSTOP tINTEGER 	{$$= new pwn::stop_node(LINE,new cdk::integer_node(LINE,$2));}
	    | tNEXT 		{$$= new pwn::next_node(LINE,new cdk::integer_node(LINE,1));}
	    | tNEXT tINTEGER 	{$$= new pwn::next_node(LINE,new cdk::integer_node(LINE,$2));}
	    ;
	    
cond: 	 tIF '(' expr ')' instr %prec tIFX 			{ $$ = new cdk::if_node(LINE, $3, $5); }
	| tIF '(' expr ')' instr tELSE instr 			{ $$ = new cdk::if_else_node(LINE, $3, $5, $7); }
	;

cicle :	 tREPEAT '(' exprs ';' exprs ';' exprs ')' instr  	{ $$ = new pwn::repeat_node(LINE, $3, $5, $7, $9); }
	;
	
exprs_args: expr ',' exprs_args {$$ = new cdk::sequence_node(LINE,$1,$3);}
	  | expr 		{$$ = new cdk::sequence_node(LINE,$1);}
	  ;

exprs: exprs_args {$$ = $1;}
      |		  {$$ = NULL;}
      ;

expr : literal			 {$$ = $1;}
     | '@'			 {$$ = new pwn::read_node(LINE);}
     | tNOOB			 {$$ = new pwn::noob_node(LINE);}
     | lval '?'			 {$$ = new pwn::addressing_node(LINE,$1); }
     | '-' expr %prec tNEG   	 {$$ = new cdk::neg_node(LINE, $2); }
     | expr '+' expr	         {$$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	         {$$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	         {$$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	         {$$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	         {$$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr	         {$$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	         {$$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	         {$$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr           	 {$$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	         {$$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	         {$$ = new cdk::eq_node(LINE, $1, $3); }
     | expr '|' expr		 {$$ = new pwn::or_node(LINE,$1,$3);}
     | expr '&' expr		 {$$ = new pwn::and_node(LINE,$1,$3);}
     | '~' expr 		 {$$ = new pwn::not_node(LINE,$2);}
     | '+' expr %prec tIDENTITY	 {$$= new pwn::identity_node(LINE,$2);}
     | '(' expr ')'              {$$ = $2;}
     | variable_expr		 {$$ = $1;}
     | lval '=' expr             {$$ = new pwn::assignment_node(LINE, $1, $3);}
     | '[' expr ']' 		 {$$= new pwn::memory_reservation_node(LINE,$2);}

     ;
     
variable_expr : lval 			{$$ = new pwn::rvalue_node(LINE,$1);}
		| funcall 		{$$ = $1;}
		;
     
lval: 	tIDENTIFIER 			{$$ = new pwn::variable_node(LINE,$1);}
	| expr '[' expr ']' 		{$$ = new pwn::index_node(LINE,$1,$3);}
	;

funcall: tIDENTIFIER '(' exprs ')'	{$$ = new pwn::call_func_node(LINE,$1,$3);}
	;
	
literal: tINTEGER {$$ = new cdk::integer_node(LINE,$1);}
	| tREAL	  {$$ = new cdk::double_node(LINE,$1);}
	| string {$$ = new cdk::string_node(LINE,$1);}
	;
	
	
string : tSTRING 	{$$ = $1;}
	| string tSTRING {$$ = new std::string(*$1 + *$2);}
	;
	
%%
