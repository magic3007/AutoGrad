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

#ifndef BOOST_AUTODIFFER_H
#define BOOST_AUTODIFFER_H

#include "IRVisitor.h"
#include "arith.h"
#include <string>
#include <stack>
#include <vector>
#include <map>

using namespace Boost::Internal;
using std::string;
using std::stack;
using std::vector;
using std::map;

class AutoDiffer : public  IRVisitor{
public:
  /**
   *
   * @param expr expression that has been be differentiated
   * @param grad_to string name of target differential variable
   * @param differential differential expression w.r.t. current expression |expr|
   * @return differential statement w.r.t. variable |grad_to_str|
   */
  Group operator ()(const Stmt &stmt, const string &grad_to_str);

protected:
  void visit(Ref<const Binary>) override;
  void visit(Ref<const Var>) override;
private:
  string grad_to_str_;
  map<string,Expr> str2old_indexes_;
  map<string,Expr> str2vars_;
  map<string,int> str2matrix_column_;
  vector<Expr> matrix_column2old_indexes_;

  vector<Expr> new_grad_to_indexes_;
  Expr new_grad_to_var_;
  stack<Expr> differentials_stack_;
  vector<Stmt> results;
};

#endif // BOOST_AUTODIFFER_H
