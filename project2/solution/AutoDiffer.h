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

#include "IRMutator.h"
#include <string>
#include <stack>

using namespace Boost::Internal;
using std::string;
using std::stack;

class AutoDiffer : public  IRMutator{
public:
  /**
   *
   * @param expr expression that has been be differentiated
   * @param grad_to string name of target differential variable
   * @param differential differential expression w.r.t. current expression |expr|
   * @return differential statement w.r.t. variable |grad_to_str|
   */
  Stmt operator ()(const Expr &expr, const string &grad_to_str,
                  const Expr &differential);
protected:
  virtual Expr visit(Ref<const IntImm>) override;
  virtual Expr visit(Ref<const FloatImm>) override;
  virtual Expr visit(Ref<const Binary>) override;
  virtual Expr visit(Ref<const Var>) override;
private:
  stack<Expr> differentials_stack_;
};

#endif // BOOST_AUTODIFFER_H
