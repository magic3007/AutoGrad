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
#include "IRHelperFunc.h"
#include "IndexReplacer.h"
#include "utils/aixlog.hpp"

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

// ================================================

class IndexCollector : public IRVisitor {
public:

  map<string, Expr> operator()(const Stmt &stmt) {
    str2index_.clear();
    stmt.visit_stmt(this);
    return str2index_;
  }

protected:
  void visit(Ref<const Index> op) override {
    string name = op->name;
    if (str2index_.find(name) == str2index_.end()) {
      str2index_[name] = op;
    }
  }

private:
  map<string, Expr> str2index_;
};

// ================================================

class VarCollector : public IRVisitor {
public:
  map<string, Expr> operator()(const Stmt &stmt) {
    str2var_.clear();
    stmt.visit_stmt(this);
    return str2var_;
  }

protected:
  void visit(Ref<const Var> op) override {
    string name = op->name;
    if (str2var_.find(name) == str2var_.end()) {
      str2var_[name] = op;
    }
  }

private:
  map<string, Expr> str2var_;
};

// ================================================

class CoeffiExtractor : public IRVisitor{
  public:
    class PackedResult{
      public:
        vector<int> coefficients;
        int imm;
    };
    PackedResult operator()(const Expr &expr, const map<string,int> &str2matrix_column){
      str2matrix_column_ = str2matrix_column;
      imm_ = 0;
      int n_cols = str2matrix_column_.size();
      coefficients_.clear();
      coefficients_.resize(n_cols, 0);
      expr.visit_expr(this, 1);
      return {coefficients_, imm_};
    }
  protected:

    void visit(Ref<const IntImm> op, int argu) override{
      imm_ += argu * op->value();
    }

    void visit(Ref<const Index> op, int argu) override{
      auto column = str2matrix_column_[op->name];
      coefficients_[column] += argu;
    }

    void visit(Ref<const Binary> op, int argu) override{
      if(op->op_type == BinaryOpType::Add){
        (op->a).visit_expr(this, argu);
        (op->b).visit_expr(this, argu);
      }else if(op->op_type == BinaryOpType::Sub){
        (op->a).visit_expr(this, argu);
        (op->b).visit_expr(this, -argu);
      }else if(op->op_type == BinaryOpType::Mul){
        if(op->a.as<IntImm>() != nullptr && op->b.as<Var>() != nullptr){
          (op->b).visit_expr(this, argu * op->a.as<IntImm>()->value());
        }else if(op->b.as<IntImm>() != nullptr && op->a.as<Var>() != nullptr){
          (op->a).visit_expr(this, argu * op->b.as<IntImm>()->value());
        }else{
          LOG(ERROR) << "Only support linear index expressions!" << std::endl;
        }
      }else{
        LOG(ERROR) << "Only support linear index expressions!" << std::endl;
      }
    }

  private:
    map<string,int> str2matrix_column_;
    vector<int> coefficients_;
    int imm_;
};
// ================================================


Group AutoDiffer::operator()(const Stmt &stmt, const string &grad_to_str) {
  grad_to_str_ = grad_to_str;
  IndexCollector index_collector;
  VarCollector var_collector;

  str2old_indexes_ = index_collector(stmt);
  str2vars_ = var_collector(stmt);

  auto lhs = stmt.as<Move>()->dst.as<Var>();
  auto differential = Var::make(lhs->type(), "d" + lhs->name, lhs->args, lhs->shape);

  str2matrix_column_.clear();
  matrix_column2old_indexes_.clear();
  {
    int counter = 0;
    for(auto &iter : str2old_indexes_){
      str2matrix_column_[iter.first] = counter++;
      matrix_column2old_indexes_.push_back(iter.second);
    }
  }

  auto grad_to_var = str2vars_[grad_to_str].as<Var>();

  /**
   * create new indexes, whose quantity is identical to that
   * of |grad_to| variable.
   */
  new_grad_to_indexes_.clear();
  auto shape =  grad_to_var->shape;
  auto indexes_quantity = shape.size();
  // FIXME: ensure that such new indexes will not conflict with existing indexes.
  string new_index_prefix = "z_";
  Type index_type = Type::int_scalar(32);
  for (size_t i = 0; i < indexes_quantity; i++){
    string new_index_name = new_index_prefix + ToString(i);
    auto extent = shape[i];
    auto dom = Dom::make(index_type,
                         IntImm::make(index_type, 0),
                         IntImm::make(index_type, extent));
    new_grad_to_indexes_.push_back(Index::make(
        index_type,
        new_index_name,
        dom,
        IndexType::Spatial
        ));
  }
  auto new_grad_to_var_name = "d" + grad_to_var->name;
  new_grad_to_var_ = Var::make(grad_to_var->type(), new_grad_to_var_name,
                               new_grad_to_indexes_, grad_to_var->shape);
  while (!differentials_stack_.empty()) differentials_stack_.pop();
  results.clear();
  results.push_back(
             LoopNest::make(new_grad_to_indexes_,
                 {Move::make(new_grad_to_var_, Expr(int32_t(0)), MoveType::MemToMem)})
             );

  differentials_stack_.emplace(differential);
  stmt.as<Move>()->src.visit_expr(this);
  return Kernel::make("grad_to_" + grad_to_var->name, {}, {},
                      results, KernelType::CPU);
}


void AutoDiffer::visit(Ref<const Binary> op){
  if(op->op_type == BinaryOpType::Add){
    (op->a).visit_expr(this);
    (op->b).visit_expr(this);
  }else if(op->op_type == BinaryOpType::Sub){
    (op->a).visit_expr(this);
    auto current_diff = differentials_stack_.top();
    auto new_differential = SimplifiedNegation(current_diff);
    differentials_stack_.emplace(new_differential);
    (op->b).visit_expr(this);
    differentials_stack_.pop();
  }else if(op->op_type == BinaryOpType::Mul){
    auto current_diff = differentials_stack_.top();
    auto new_differential = SimplifiedMultiplication(op->b, current_diff);
    differentials_stack_.emplace(new_differential);
    (op->a).visit_expr(this);
    differentials_stack_.pop();

    new_differential = SimplifiedMultiplication(op->a, current_diff);
    differentials_stack_.emplace(new_differential);
    (op->b).visit_expr(this);
    differentials_stack_.pop();

  }else if (op->op_type == BinaryOpType::Div){
    /**
     * TODO: We only support the situation where the denominator is an immediate currently.
     * However, this code could be modified to support the case where the denominator
     * consists of variable with ease. :)
     */
    LOG(ERROR) << COND(op->b.as<FloatImm>()==nullptr &&
        op->b.as<IntImm>()==nullptr &&
        op->b.as<UIntImm>()==nullptr)
               << "We only support the situation " \
                  "where the denominator is an immediate currently." << std::endl;

    auto current_diff = differentials_stack_.top();
    // FIXME: use expression |current_diff|'s type by default
    auto new_differential = SimplifiedDivision(current_diff, op->b);
    differentials_stack_.emplace(new_differential);
    (op->a).visit_expr(this);
    differentials_stack_.pop();
  }else{
    LOG(ERROR) << "Unsupported Binary operation." << std::endl;
  }
}

void AutoDiffer::visit(Ref<const Var> op) {
  if(op->name != grad_to_str_){
    return;
  }
  CoeffiExtractor coeffi_extractor;

  vector<vector<int>> coefficients{};
  vector<Expr> rhs{};
  for(size_t i = 0; i < op->args.size(); i++){
     auto rv = coeffi_extractor(op->args[i], str2matrix_column_);
     rhs.push_back(SimplifiedSubtraction(new_grad_to_indexes_[i], Expr(int32_t(rv.imm))));
     coefficients.push_back(rv.coefficients);
  }
  using namespace arithmetic::gaussian_elimination;
  auto matrix = Matrix::make(coefficients, rhs);
  ImplGaussianEliminationMethod impl_gaussian_elimination_method;
  auto rv = impl_gaussian_elimination_method(matrix, matrix_column2old_indexes_);

  IndexReplacer index_replacer;
  auto current_diff = index_replacer(differentials_stack_.top(), rv.solutions);

  if(!rv.constraints.empty()){
    Expr cond = rv.constraints[0];
    for(size_t i = 1; i < rv.constraints.size(); i++){
      auto cond_type = Type::int_scalar(32);
      cond = Binary::make(cond_type, BinaryOpType::And, cond, rv.constraints[i]);
    }
    current_diff = Select::make(current_diff->type(), cond, current_diff, Expr(int32_t(0)));
  }
  auto diff_stmt = Move::make(new_grad_to_var_,current_diff, MoveType::MemToMem);
  IndexCollector index_collector;
  map<string, Expr> new_index_map = index_collector(diff_stmt);
  vector<Expr> new_index_list{};
  new_index_list.reserve(new_index_map.size());
  for(auto& iter : new_index_map) new_index_list.push_back(iter.second);
  diff_stmt = LoopNest::make(new_index_list, {diff_stmt});
  results.push_back(diff_stmt);
}
