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

#include "IndexAnalyst.h"
#include "utils/aixlog.hpp"

map<string, Expr> IndexAnalyst::operator()(const Stmt &stmt) {
  domains_.clear();
  index_types_.clear();
  is_rhs = false;
  stmt.visit_stmt(this);
  map<string, Expr> rv{};
  Type index_t = Type::int_scalar(32);
  for (auto &iter : domains_) {
    auto index_name = iter.first;
    auto upper_bound = iter.second;
    auto index_type = index_types_[index_name];
    auto dom = Dom::make(index_t, 0, upper_bound);
    auto index = Index::make(index_t, index_name, dom, index_type);
    rv.emplace(index_name, index);
  }
  return rv;
}

void IndexAnalyst::visit(Ref<const Move> op) {
  is_rhs = false;
  (op->dst).visit_expr(this);
  is_rhs = true;
  (op->src).visit_expr(this);
}

void IndexAnalyst::visit(Ref<const Var> op) {
  for (size_t i = 0; i < op->args.size(); i++) {
    op->args[i].visit_expr(this, op->shape[i]);
  }
}

void IndexAnalyst::visit(Ref<const Binary> op, int argu) {
  auto p = op->b.as<IntImm>();
  if (p != nullptr) { // rhs is an integer immediate
    if (op->op_type == BinaryOpType::Add) {
      argu -= p->value();
    } else if (op->op_type == BinaryOpType::Mul) {
      argu /= p->value();
    } else if (op->op_type == BinaryOpType::IntDiv) {
      argu *= p->value();
    } else if (op->op_type == BinaryOpType::Mod) {
      return;
    }
  }
  (op->a).visit_expr(this, argu);
  (op->b).visit_expr(this, argu);
}

void IndexAnalyst::visit(Ref<const Var> op, int argu) {
  auto domain = domains_.find(op->name);
  if (domain == domains_.end()) {
    if (is_rhs) {
      index_types_[op->name] = IndexType::Reduce;
    } else {
      index_types_[op->name] = IndexType::Spatial;
    }
    domains_[op->name] = argu;
  } else {
    domains_[op->name] = std::min(domains_[op->name], argu);
  }
}
