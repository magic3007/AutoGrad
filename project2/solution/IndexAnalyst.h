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

#ifndef BOOST_INDEXANALYST_H
#define BOOST_INDEXANALYST_H

#include "IRVisitor.h"
#include <vector>
#include <map>
#include <string>

using namespace Boost::Internal;
using std::vector;
using std::map;
using std::string;

class IndexAnalyst : public IRVisitor{
public:
  map<string, Expr> operator ()(const Stmt &stmt);
protected:
  virtual void visit(Ref<const Move> op) override;
  virtual void visit(Ref<const Var> op) override;
  virtual void visit(Ref<const Binary> op, int argu) override;
  virtual void visit(Ref<const Var> op, int argu) override;
private:
  bool  is_rhs;
  map<string, IndexType> index_types_;
  map<string, int> domains_;
};

#endif // BOOST_INDEXANALYST_H
