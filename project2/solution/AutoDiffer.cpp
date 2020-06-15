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

#include "AutoDiffer.h"
#include "IRVisitor.h"
#include <string>
#include <vector>
#include "utils/aixlog.hpp"

using std::string;
using std::vector;


// ================================================
// healper functions
// ================================================

template <typename T>
string ToString(const T &val){
  std::stringstream ss;
  ss << val;
  return ss.str();
}

bool IsExprEqualZero(const Expr &expr){
  auto e = expr.as<IntImm>();
  return e!=nullptr && e->value() == 0;
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

// ================================================

class VarCollector : public IRVisitor {
public:
  Ref<const Var> operator()(const Expr &expr, const string &name) {
    name_ = name;
    rv_.set_ptr(nullptr);
    have_collected_ = false;
    expr.visit_expr(this);
    return rv_;
  }

protected:
  void visit(Ref<const Var> op) {
    if (!have_collected_ && op->name == name_) {
      have_collected_ = true;
      rv_ = op;
    }
  }

private:
  string name_;
  bool have_collected_ = false;
  Ref<const Var> rv_;
};

// ================================================

Stmt AutoDiffer::operator()(const Expr &expr, const string &grad_to_str,
                            const Expr &differential) {
  VarCollector var_collector;
  auto grad_to_var = var_collector(expr, grad_to_str);

  /**
   * create new indexes, whose quantity is identical to that
   * of |grad_to| variable.
   */
  auto shape =  grad_to_var->shape;
  auto indexes_quantity = shape.size();
  vector<Expr> new_indexes{};
  // FIXME: ensure that such new indexes will not conflict with existing indexes.
  string new_index_prefix = "z_";
  Type index_type = Type::int_scalar(32);
  for (size_t i = 0; i < indexes_quantity; i++){
    string new_index_name = new_index_prefix + ToString(i);
    auto extent = shape[i];
    auto dom = Dom::make(index_type,
                         IntImm::make(index_type, 0),
                         IntImm::make(index_type, extent));
    new_indexes.push_back(Index::make(
        index_type,
        new_index_name,
        dom,
        IndexType::Spatial
        ));
  }

  while (!differentials_stack_.empty())
    differentials_stack_.pop();
  differentials_stack_.emplace(differential);
  auto differentiated_expr = expr.mutate_expr(this);

  return Stmt();
}

Expr AutoDiffer::visit(Ref<const IntImm> op) {
  return IntImm::make(op->type(), 0);
}

Expr AutoDiffer::visit(Ref<const FloatImm> op) {
  return FloatImm::make(op->type(), 0.0);
}

Expr AutoDiffer::visit(Ref<const Binary> op){
  Expr diff_a, diff_b, diff_rv;
  if(op->op_type == BinaryOpType::Add){
    diff_a = mutate(op->a);
    diff_b = mutate(op->b);
    diff_rv = SimplifiedAddition(diff_a, diff_b);
  }else if(op->op_type == BinaryOpType::Sub){
    diff_a = mutate(op->a);
    diff_b = mutate(op->b);
    if(IsExprEqualZero(diff_b)) {
      diff_rv = diff_a;
    }else if (IsExprEqualZero(diff_a)){
      diff_rv = Unary::make(diff_b->type(), UnaryOpType::Neg, diff_b);
      diff_rv = Bracket::make(diff_rv);
    }else{
      // FIXME: use expression |diff_a|'s type by default
      diff_rv = Binary::make(diff_a->type(), BinaryOpType::Sub, diff_a ,diff_b);
    }
  }else if(op->op_type == BinaryOpType::Mul){
    auto current_diff = differentials_stack_.top();
    // FIXME: use expression |current_diff|'s type by default

    auto new_differential = Binary::make(current_diff->type(),
      BinaryOpType::Mul, op->b, current_diff);
    diff_a = mutate(op->a);
    differentials_stack_.emplace(new_differential);
    differentials_stack_.pop();

    new_differential = Binary::make(current_diff->type(),
      BinaryOpType::Mul, op->a, current_diff);
    diff_b = mutate(op->b);
    differentials_stack_.emplace(new_differential);
    differentials_stack_.pop();

    diff_rv = SimplifiedAddition(diff_a, diff_b);

  }else if (op->op_type == BinaryOpType::Div){
    /**
     * TODO: We only support the situation where the denominator is an immediate currently.
     * However, this code could be modified to support the case where the denominator
     * consists of variable with ease. :)
     */
    LOG(ERROR) << COND(op->b.as<FloatImm>()!=nullptr &&
        op->b.as<IntImm>()!=nullptr &&
        op->b.as<UIntImm>()!=nullptr)
               << "We only support the situation " \
                  "where the denominator is an immediate currently." << std::endl;

    auto current_diff = differentials_stack_.top();
    // FIXME: use expression |current_diff|'s type by default

    auto new_differential = Binary::make(current_diff->type(),
      BinaryOpType::Div, current_diff, op->b);
    diff_rv = mutate(op->a);
    differentials_stack_.emplace(new_differential);
    differentials_stack_.pop();
      }else{
    LOG(ERROR) << "Unsupported Binary operation." << std::endl;
  }
  return diff_rv;
}
