# Compiler Project(2020 Spring): Code Generation Compiler

> 苏灿 1700012779 凌子轩 1700012752
> 麦景 1700012751 张灏宇 1700011044

## Overview

本项目的主要工作是解析输入的`json`类型的文件并根据表达式文法规则构建`AST`，然后完成`c++`代码的翻译工作.

### Usage

#### Dependencies

- [flex](https://github.com/westes/flex/): a fast lexical analyzer - scanner generator for lexing in C and C++
- [bison](https://www.gnu.org/software/bison/): a general-purpose parser generator that converts an annotated context-free grammar into a deterministic LR or generalized LR (GLR) parser employing LALR(1) parser tables. 
- [nlohmann/json](https://github.com/nlohmann/json): JSON Parser for Modern C++
    - In this project, we use the single required header file [`json.hpp`](https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp) 

*Note: 此项目还修改了除了solutions文件夹下的其他内容, 但是不会影响最终评测.* 

##### Ubuntu 18.04
```bash
apt-get -y install bison flex
```
#### Compilation & Test

```bash
mkdir build && cd build
cmake ..
make -j4
./project1/test1
```
demo见[此](http://showterm.io/9f6041d5b4a1c5b365b99).


### Docker

此外, 为防止编译环境不同造成的错误, 我们还提供了一个统一的docker环境, 详细见[docker/Dockerfile.remote-cpp-env](../docker/Dockerfile.remote-cpp-env)文件前部的注释.
```dockerfile
# CLion remote docker environment (How to build docker container, run and stop it)
#
# Build and run:
#   docker build -t clion/remote-cpp-env-boost:0.5 -f Dockerfile.remote-cpp-env .
#   docker run -d --cap-add sys_ptrace -p127.0.0.1:2233:22 --name clion_remote_env-boost clion/remote-cpp-env-boost:0.5
#   ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:2233" 
#
# ssh credentials (test user):
#   ssh user@localhost -p 2233
#
# restart:
#   docker restart clion_remote_env-boost
# stop:
#   docker stop clion_remote_env-boost
# remove:
#   docker rm clion_remote_env-boost
# 
```

## 设计思路及实现方法


### AST的生成

在使用`json`库从输入文件中解析生成函数签名的基础上，我们查阅语相应语法规范编写了[grammar.y](../project1/solutions/grammar.y)和[token.l](../project1/solutions/token.l)两个语法文件，通过`flex` 和`bison`工具生成`token.cc`和`grammar.cc`两个文件，该文件可用于生成AST。具体的实现如下：

#### token.l

```c++
// token.l

static FILE *yyparse_file_ptr;

void set_yyparse_file_ptr(FILE *file_ptr){
    yyparse_file_ptr = file_ptr;
    yyin = yyparse_file_ptr;
}
```

在`token.l`文件中定义了上述静态函数用来提供语法分析数的输入接口，调用该函数使用输入文件指针作为参数即可。在该文件中，接下来根据表达式文法，生成了相应的`token`定义：

```c++
// token.l

/* don't invoke function "yywrap" when encountering EOF */
%option noyywrap

/* regular definitions */
delim       [ \t\r\n]
ws          {delim}+
letter      [a-zA-Z]
digit       [0-9]
id          [a-zA-Z_][a-zA-Z0-9_]*
float       {digit}+\.{digit}+
integer     {digit}+

%%

{ws}                        { /* no action and no return */ }

{id}                        { SAVE_TOKEN; return TIDENTIFIER; }
{float}                     { SAVE_TOKEN; return TFLOAT; }
{integer}                   { SAVE_TOKEN; return TINTEGER; }

";"                         { return TOKEN(TSEMICOLON); }
"="                         { return TOKEN(TEQUAL); }
","                         { return TOKEN(TCOMMA); }
"["                         { return TOKEN(TLBRACKET); }
"]"                         { return TOKEN(TRBRACKET); }
"("                         { return TOKEN(TLPAREN); }
")"                         { return TOKEN(TRPAREN); }
"<"                         { return TOKEN(TCLT); }
">"                         { return TOKEN(TCGT); }
"+"                         { return TOKEN(TPLUS); }
"-"                         { return TOKEN(TMINUS); }
"*"                         { return TOKEN(TMUL); }
"//"                        { return TOKEN(TINTDIV); }
"/"                         { return TOKEN(TDIV); }
"%"                         { return TOKEN(TMOD); }
.                           { fprintf(stderr, "Unknown token:%s\n", yytext); yyterminate();}

%%
```



#### grammar.y文件

在该文件中定义了`Group root`变量，作为生成的语法树的根节点最终返回。

```c++
// grammar.y

%union  /* define stack type */
{
    Stmt*           stmt;
    Expr*               expr;
    std::vector<Stmt>*      stmt_vec;
    std::vector<Expr>*      expr_vec;
    std::vector<size_t>*    int_vec;
    std::string*        string;
    int             token;
}
```

文件中的该`union`结构，定义了语法分析的过程中，栈中的元素结构，因为栈中放的元素可以有上述几种类型，所以如此定义。

```c++
// grammar.y

%type <stmt>        Stmt
%type <expr>        Rhs TRef SRef IdExpr
%type <stmt_vec>    StmtList
%type <expr_vec>    AList
%type <int_vec>     CList
```

在该文件中如上定义了表达式文法中的符号类型。

在接下来的`kernel`部分中定义表达式文法每个生成式的具体处理方式：

```c++
// grammar.y

Rhs
    : Rhs TPLUS Rhs
        {
            // FIXME: arbitrary data type
            Type data_type = Type::float_scalar(32);
            $$ = new Expr(Binary::make(data_type, BinaryOpType::Add, *$1, *$3));
            delete $1;
            delete $3;
        }
```

在如上的代码段中给出了`Rhs ::= Rhs_1 + Rhs_2`生成式的处理方式，其中`$1,$3`分别代表生成式右边的第一项和第三项，即`Rhs_1,Rhs_2`，而`$$`代表生成式左边项，即`Rhs`。在该处理中首先给定了数据的类型为`float`，然后给定参数实例化一个`Expr`类的变量赋值给`$$`，即生成式左端项。

对于该表达式文法的其他每个生成式，类似上面的方式处理即可。

#### parser.h

通过上述方式得到了AST的生成文件后，再编写了`parser.h`文件，在该文件中给定了AST的几个调用接口，在本项目中我们使用如下接口：

```c++
// parser.h

Group ParseFromString(const string &text, int verbose=0){
  auto file_ptr = tmpfile();
  fputs(text.c_str(), file_ptr);
  rewind(file_ptr);
  set_yyparse_file_ptr(file_ptr);
  yydebug = verbose;
  yyparse();
  fclose(file_ptr);
  return root;
}
```

该接口函数传入表达式字符串，返回生成的AST的根节点。


### 生成函数签名
在实现了AST的基础上，定义并实现了类`signPrinter`继承给出的`IRPrinter`类，用于生成字符串形式的函数签名中的参数部分。本部分代码位于`signPrint.h`和`signPrint.cc`中。
这个类的构造函数有三个参数，是解析json文件中得到的`ins`，`outs`及`data_type`的内容，分别记录了函数的输入输出参数和数据类型：

```c++
// signPrinter.h

signPrinter(std::vector<std::string> _ins, std::vector<std::string> _outs, std::string _type) : IRPrinter() 
{
    ins = _ins;outs=_outs;type=_type;
}
```
类里有一个私有成员属性`map<string,string> ranges`，用来记录一个变量名到它的数组大小的映射，数组大小用一个字符串记录。如果该变量不是数组，则ranges里映射的为空字符串。在输出函数中，首先扫描ins里的元素，并生成对应字符串；然后扫描outs里的元素，并检查它是否在ins里面，若不在则生成对应字符串。

最后参数部分与解析json得到的函数名部分组合，生成函数签名。

`signPrinter`中的主要函数如下：

```c++
std::map<std::string, std::string> ranges;
std::vector<std::string> ins;
std::vector<std::string> outs;
std::string type;

void signPrinter::visit(Ref<const Var> op) {
    std::string name = op->name;
    if (op->shape.size() == 1 && op->shape[0] == 1) //该变量不是数组
    {
        ranges[name] = "";
        return;
    }
    //该变量是数组
    std::string size = "[";

    for (size_t j = 0; j < op->shape.size(); ++ j) {
        size = size + std::to_string(op->shape[j]);
        if (j < op->shape.size() - 1) {
            size = size +  "][";
        }
    }
    size += "]";
    ranges[name] = size;
}

std::string signPrinter::print(const Group &group) {
    ranges.clear();
    group.visit_group(this);
    std::string ret = "(";
    bool first = 1;
    for (size_t i = 0; i < ins.size(); ++ i)
    {
        std::string name = ins[i];
        std::string size = ranges[name];
        if (!first)
            ret += ", ";
        ret += type + " ";
        first = 0;
        if (size.length() == 0)
            ret += "&" +name;
        else
            ret += "(&"+name+")"+size;
    }
    for (size_t i = 0; i < outs.size(); ++ i)
    {
        bool flag = 1;
        for (size_t j = 0; j < ins.size() && flag; ++ j)
            if (ins[j] == outs[i]) flag = 0;
		if (!flag) continue;
        std::string name = outs[i];
        std::string size = ranges[name];
        if (!first)
            ret += ", ";
        ret += type + " ";
        first = 0;
        if (size.length() == 0)
            ret += "&" +name;
        else
            ret += "(&"+name+")"+size;   
    }
    ret += ")";
    return ret;
}
```
通过生成的`signPrinter`类提供的`print`接口即可解析语法分析树，生成函数签名。

### 生成函数主体

在实现了AST的基础上，定义并实现了类`CPPPrinter`继承给出的`IRPrinter`类，该类用于生成最终的C++代码。本部分的代码位于`CPPPrinter.h`和`CPPPrinter.cc`文件中。

根据目标表达式文法的需要，重载`IRPrinter`中的部分`visitor`函数：

```c++
// 重载的visit函数列表
void visit(Ref<const IntImm>) override;
void visit(Ref<const FloatImm>) override;
void visit(Ref<const Binary>) override;
void visit(Ref<const Var>) override;
void visit(Ref<const Move>) override;
void visit(Ref<const Kernel>) override;
void visit(Ref<const Bracket>) override;

// 下面是带参数的visit
void visit(Ref<const IntImm>, int) override;
void visit(Ref<const Binary>, int) override;
void visit(Ref<const Var>, int) override;
```

在`CPPPrinter.cc`实现了相应`visit`函数，在实现中特殊处理的地方是最终生成的循环中数组元素的参数范围的确定。具体实现时重新实现了一组带参数的`visit`函数，在上述函数说明中也有标注。

这些`visit`在实现时传入的参数是数组对应维的大小，该大小从`AST`的对应节点中获得。具体实现如下：

```c++
void CPPPrinter::visit(Ref<const IntImm> op, int argu) { oss << op->value(); }

void CPPPrinter::visit(Ref<const Binary> op, int argu) {
    // 如果Binary的右表达式是立即数，则根据立即数和运算符更新左表达式的bound
    // 仅考虑运算符为 + * // 的情况，因为 % 并非单调运算
    auto p = std::dynamic_pointer_cast<const IntImm>(op->b.real_ptr());
    if (p != nullptr) {
        if (op->op_type == BinaryOpType::Add) {
            argu -= p->value();
        } else if (op->op_type == BinaryOpType::Mul) {
            argu /= p->value();
        } else if (op->op_type == BinaryOpType::IntDiv) {
            argu *= p->value();
        }
    }
    (op->a).visit_expr(this, argu);
    if (op->op_type == BinaryOpType::Add) {
        oss << " + ";
    } else if (op->op_type == BinaryOpType::Sub) {
        oss << " - ";
    } else if (op->op_type == BinaryOpType::Mul) {
        oss << " * ";
    } else if (op->op_type == BinaryOpType::Div) {
        oss << " / ";
    } else if (op->op_type == BinaryOpType::Mod) {
        oss << " % ";
    } else if (op->op_type == BinaryOpType::IntDiv) {
        oss << " / ";
    }

    (op->b).visit_expr(this, argu);
}

void CPPPrinter::visit(Ref<const Var> op, int argu) {
    oss << op->name;
    // ranges为维护<variable, bound>关系的Map
    // 根据传入的范围参数argu更新bound
    auto r = ranges.find(op->name);
    if (r == ranges.end()) {
        indices.push_back(op->name);
        ranges[op->name] = argu;
    } else {
        int t = ranges[op->name];
        if (argu < t) ranges[op->name] = argu;
    }
}
```

## Division of Labor within the Group

| Name     |Student ID|  Labor   |
| -------- | -------- | -------- |
| 麦景      | 1700012751     | generate AST by leveraging `lex` & `yacc`  |
| 苏灿 |1700012779  | generate the function body |
| 凌子轩 | 1700012752 | generate the function signature |
| 张灏宇 | 1700011044 | program linking and report summary|