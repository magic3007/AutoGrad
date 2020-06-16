/*
MIT License

Copyright (c) 2020 MagicMai

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef BOOST_IRHELPERFUNC_H

#include "IR.h"
#include <string>

// ================================================
// healper functions
// ================================================

using namespace Boost::Internal;
using std::string;

template <typename T>
string ToString(const T &val){
  std::stringstream ss;
  ss << val;
  return ss.str();
}

template<typename T>
bool IsExprEqualIntImm(const Expr &expr, T a){
  auto e = expr.as<IntImm>();
  return e!=nullptr && e->value() == a;
}

bool IsExprEqualZero(const Expr &expr){
  return IsExprEqualIntImm(expr, 0);
}

Expr SimplifiedNegation(const Expr &a){
  if(IsExprEqualZero(a)){
    return a;
  }else{
    return Bracket::make(Unary::make(a->type(), UnaryOpType::Neg, Bracket::make(a)));
  }
}

Expr SimplifiedAddition(const Expr &a, const Expr &b){
  if(IsExprEqualZero(a)){
    return b;
  }else if (IsExprEqualZero(b)){
    return a;
  }else{
    // FIXME: use expression |a|'s type by default
    return Binary::make(a->type(), BinaryOpType::Add, a ,b);
  }
}

Expr SimplifiedSubtraction(const Expr &a, const Expr &b){
  if(IsExprEqualZero(a)) return SimplifiedNegation(b);
  if (IsExprEqualZero(b)) return a;
  // FIXME: use expression |a|'s type by default
  return Binary::make(a->type(), BinaryOpType::Sub, a ,Bracket::make(b));
}

Expr SimplifiedMultiplication(const Expr &a, const Expr &b){
  if(IsExprEqualZero(a) || IsExprEqualZero(b)) return Expr(int32_t(0));
  if (IsExprEqualIntImm(a, 1)) return b;
  if (IsExprEqualIntImm(b, 1)) return a;
  if (IsExprEqualIntImm(a, -1)) return SimplifiedNegation(b);
  if (IsExprEqualIntImm(b, -1)) return SimplifiedNegation(a);

  // FIXME: use expression |a|'s type by default
  return Binary::make(a->type(), BinaryOpType::Mul, Bracket::make(a) ,Bracket::make(b));
}

Expr SimplifiedDivision(const Expr &a, const Expr &b){
  if(IsExprEqualZero(a)) return Expr(int32_t(0));
  if (IsExprEqualIntImm(b, 1)) return a;
  if (IsExprEqualIntImm(b, -1)) return SimplifiedNegation(a);

  // FIXME: use expression |a|'s type by default
  return Binary::make(a->type(), BinaryOpType::Div, Bracket::make(a) ,Bracket::make(b));
}

#define BOOST_IRHELPERFUNC_H

#endif // BOOST_IRHELPERFUNC_H
