%{
#include <stdio.h>
#include "IR.h"

using namespace Boost::Internal;

Group root;

%}

/* Credit to https://www.gnu.org/software/bison/manual/html_node/Token-Values.html. */
/* %union in yacc/bison only allows POD types...hmm */
%union  /* define stack type */
{
    Kernel kernel;
    Stmt   stmt;
    Expr   expr;
    std::vector<Expr> expr_vec;
    std::vector<size_t> int_vec;
    std::string string;
    int token;
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
%type <kernel>      Kernel
%type <stmt>        Stmt
%type <expr>        Rhs TRef SRef IdExpr
%type <expr_vec>    AList
%type <int_vec>     CList

%left TPLUS TMINUS
%left TMUL TDIV TMOD TINTDIV

%start Program

%%

Program
    : Kernel
        {
            root = $1;
        }
    ;

Kernel
    : Stmt
        {
            // FIXME: ignore passing parameters |_inputs| & |_outputs|
            $$ = Kernel::make("kernel", {}, {}, {$1}, KernelType::CPU);
        }
    | Kernel Stmt
        {
            auto stmt_list = $1->stmt_list;
            stmt_list.push_back($2);
            // FIXME: ignore passing parameters |_inputs| & |_outputs|
            $$ = Kernel::make("kernel", {}, {}, stmt_list, KernelType::CPU);
        }
    ;

Stmt
    : TRef TEQUAL Rhs TSEMICOLON
        {
            $$ = Move::make($1, $3, MoveType::MemToMem);
        }
    ;

Rhs
    : Rhs TPLUS Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = Binary::make(data_type, BinaryOpType::Add, $1, $3);
        }
    | Rhs TMINUS Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = Binary::make(data_type, BinaryOpType::Sub, $1, $3);
        }
    | Rhs TMUL Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = Binary::make(data_type, BinaryOpType::Mul, $1, $3);
        }
    | Rhs TDIV Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = Binary::make(data_type, BinaryOpType::Div, $1, $3);
        }
    | Rhs TINTDIV Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = Binary::make(data_type, BinaryOpType::IntDiv, $1, $3);
        }
    | Rhs TMOD Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = Binary::make(data_type, BinaryOpType::Mod, $1, $3);
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
            $$ = IntImm::make(type, atol($1->c_str()));
        }
    | TFLOAT
        {
            // FIXME: arbitrary data type
            Type type = Type::float_scalar(32);
            $$ = FloatImm::make(type, atof($1->c_str()));
        }
    ;

SRef
    : TIDENTIFIER TCLT CList TCGT
        {
            // FIXME: use arbitrary data type && ignore passing parameters |_shape|
            Type type = Type::float_scalar(32);
            $$ = Var::make(type, $1, {}, $3);
        }
    ;

TRef
    : TIDENTIFIER TCLT CList TCGT TLBRACKET AList TRBRACKET
        {
            // FIXME: use arbitrary data type
            Type type = Type::float_scalar(32);
            $$ = Var::make(type, $1, $6, $3);
        }
    ;

CList
    : CList TCOMMA TINTEGER
        {
            lst = $1;
            lst.push_back(atol($3->c_str()));
            $$ = lst;
        }
    | TINTEGER
        {
            $$ = {atol($1->c_str())};
        }
    ;

AList
    : AList TCOMMA IdExpr
        {
            lst = $1;
            lst.push_back($3);
            $$ = lst;
        }
    | IdExpr
        {
            $$ = {$1};
        }
    ;

IdExpr
    : TIDENTIFIER
        {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            $$ = Var::make(type, $1, {}, {});
        }
    | IdExpr TPLUS IdExpr
        {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            $$ = Expr::make(type, BinaryOpType::Add, $1 $3);
        }
    | IdExpr TPLUS TINTEGER
        {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            auto imm = IntImm::make(type, atol($3->c_str()));
            $$ = Expr::make(type, BinaryOpType::Add, $1, imm);
        }
    | IdExpr TMUL TINTEGER
         {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            auto imm = IntImm::make(type, atol($3->c_str()));
            $$ = Expr::make(type, BinaryOpType::Mul, $1, imm);
        }
    | IdExpr TINTDIV TINTEGER
         {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            auto imm = IntImm::make(type, atol($3->c_str()));
            $$ = Expr::make(type, BinaryOpType::IntDiv, $1, imm);
        }
    | IdExpr TMOD TINTEGER
         {
            // FIXME: use arbitrary data type
            Type type = Type::int_scalar(32);
            auto imm = IntImm::make(type, atol($3->c_str()));
            $$ = Expr::make(type, BinaryOpType::Mod, $1, imm);
        }
    ;

%%

int yyerror(const char* msg){
    fprintf(stderr, "Error: %s encountered at line number: %d\n", msg, yylineno);
 }