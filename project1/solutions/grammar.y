%{
#include <stdio.h>
#include <memory>
#include "IR.h"

using namespace Boost::Internal;

Group root;

extern int yylineno;
extern int yylex();

void yyerror(const char* msg){
    fprintf(stderr, "Error: %s encountered at line number: %d\n", msg, yylineno);
}

using std::vector;
using std::string;

%}


/**
// Credit to https://www.gnu.org/software/bison/manual/html_node/Token-Values.html.
// %union in yacc/bison only allows POD types...hmm
// Although std::string is definitely not POD, std::string* is.
// (Any pointer is a POD type, regardless how non-plain the object it points to.)
// See more info in https://stackoverflow.com/questions/26964941/lex-yacc-no-member-name-and-declaration-error
*/
%union  /* define stack type */
{
    Stmt*  			stmt;
    Expr*   			expr;
    std::vector<Stmt>*		stmt_vec;
    std::vector<Expr>* 		expr_vec;
    std::vector<size_t>* 	int_vec;
    std::string* 		string;
    int 			token;
}

/*
    Credit to https://www.gnu.org/software/bison/manual/html_node/Token-Decl.html
    (terminal symbols)
*/
%token <string> TIDENTIFIER TINTEGER TFLOAT
%token <token>  TSEMICOLON TCOMMA
%token <token>  TEQUAL
%token <token>  TLBRACKET TRBRACKET TCLT TCGT
%token <token>  TPLUS TMINUS TMUL TDIV TINTDIV TMOD

/*
    Credit to https://www.gnu.org/software/bison/manual/html_node/Type-Decl.html
    (non-terminal symbols)
*/
%type <stmt>        Stmt
%type <expr>        Rhs TRef SRef IdExpr
%type <stmt_vec>    StmtList
%type <expr_vec>    AList
%type <int_vec>     CList

%left TPLUS TMINUS
%left TMUL TDIV TMOD TINTDIV

%start Kernel

%%

Kernel
    : StmtList
        {
            // FIXME: ignore passing parameters |_inputs| & |_outputs|
            root = Kernel::make("kernel", {}, {}, {*$1}, KernelType::CPU);
            delete $1;
        }
    ;

StmtList
    : Stmt
        {
            $$ = new vector<Stmt>();
            $$->push_back(*$1);
            delete $1;
        }
    | StmtList Stmt
        {
            $$ = $1;
            $$->push_back(*$2);
            delete $2;
        }
    ;

Stmt
    : TRef TEQUAL Rhs TSEMICOLON
        {
		$$ = new Stmt(Move::make(*$1, *$3, MoveType::MemToMem));
		delete $1;
		delete $3;
        }
    ;

Rhs
    : Rhs TPLUS Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = new Expr(Binary::make(data_type, BinaryOpType::Add, *$1, *$3));
            delete $1;
            delete $3;
        }
    | Rhs TMINUS Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = new Expr(Binary::make(data_type, BinaryOpType::Sub, *$1, *$3));
            delete $1;
            delete $3;
        }
    | Rhs TMUL Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = new Expr(Binary::make(data_type, BinaryOpType::Mul, *$1, *$3));
            delete $1;
            delete $3;
        }
    | Rhs TDIV Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = new Expr(Binary::make(data_type, BinaryOpType::Div, *$1, *$3));
            delete $1;
            delete $3;
        }
    | Rhs TINTDIV Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = new Expr(Binary::make(data_type, BinaryOpType::IntDiv, *$1, *$3));
            delete $1;
            delete $3;
        }
    | Rhs TMOD Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = new Expr(Binary::make(data_type, BinaryOpType::Mod, *$1, *$3));
            delete $1;
            delete $3;
        }
    | TRef
        {
            $$ = $1;
        }
    | SRef
        {
            $$ = $1;
        }
    | TINTEGER
        {
            // FIXME: arbitrary data type
            Type type = Type::int_scalar(32);
            $$ = new Expr(IntImm::make(type, atol($1->c_str())));
            delete $1;
        }
    | TFLOAT
        {
            // FIXME: arbitrary data type
            Type type = Type::float_scalar(32);
            $$ = new Expr(IntImm::make(type, atof($1->c_str())));
            delete $1;
        }
    ;

SRef
    : TIDENTIFIER TCLT CList TCGT
        {
            // FIXME: use arbitrary data type && ignore passing parameters |_shape|
            Type type = Type::float_scalar(32);
            $$ = new Expr(Var::make(type, *$1, {}, *$3));
            delete $1;
            delete $3;
        }
    ;

TRef
    : TIDENTIFIER TCLT CList TCGT TLBRACKET AList TRBRACKET
        {
            // FIXME: use arbitrary data type
            Type type = Type::float_scalar(32);
            $$ = new Expr(Var::make(type, *$1, *$6, *$3));
            delete $1;
            delete $3;
            delete $6;
        }
    ;

CList
    : CList TCOMMA TINTEGER
        {
            $$ = $1;
            $$->push_back(atol($3->c_str()));
            delete $3;
        }
    | TINTEGER
        {
            $$ = new vector<size_t>();
            $$->push_back(atol($1->c_str()));
            delete $1;
        }
    ;

AList
    : AList TCOMMA IdExpr
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    | IdExpr
        {
            $$ = new vector<Expr>();
            $$->push_back(*$1);
            delete $1;
        }
    ;

IdExpr
    : TIDENTIFIER
        {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            $$ = new Expr(Var::make(type, *$1, {}, {}));
            delete $1;
        }
    | IdExpr TPLUS IdExpr
        {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            $$ = new Expr(Binary::make(type, BinaryOpType::Add, *$1, *$3));
            delete $1;
            delete $3;
        }
    | IdExpr TPLUS TINTEGER
        {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            auto imm = IntImm::make(type, atol($3->c_str()));
            $$ = new Expr(Binary::make(type, BinaryOpType::Add, *$1, imm));
            delete $1;
            delete $3;
        }
    | IdExpr TMUL TINTEGER
         {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            auto imm = IntImm::make(type, atol($3->c_str()));
            $$ = new Expr(Binary::make(type, BinaryOpType::Mul, *$1, imm));
            delete $1;
            delete $3;
        }
    | IdExpr TINTDIV TINTEGER
         {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            auto imm = IntImm::make(type, atol($3->c_str()));
            $$ = new Expr(Binary::make(type, BinaryOpType::IntDiv, *$1, imm));
            delete $1;
            delete $3;
        }
    | IdExpr TMOD TINTEGER
         {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            auto imm = IntImm::make(type, atol($3->c_str()));
            $$ = new Expr(Binary::make(type, BinaryOpType::Mod, *$1, imm));
            delete $1;
            delete $3;
        }
    ;

%%
