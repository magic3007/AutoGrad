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

#include "arith.h"
#include "IRHelperFunc.h"
#include "utils/aixlog.hpp"

namespace arithmetic {

namespace gaussian_elimination {

RefMatrixRow MatrixRow::make(const vector<int> &coefficients, const Expr &rhs) {
  return std::make_shared<MatrixRow>(coefficients, rhs);
}

RefMatrixRow operator+(RefMatrixRow a, RefMatrixRow b) {
  LOG(DEBUG) << COND(a->n_cols_ == b->n_cols_) << "Mismatch column number"
             << std::endl;
  vector<int> new_coefficients(a->n_cols_);
  for (size_t i = 0; i < a->n_cols_; i++) {
    new_coefficients[i] = a->coefficients_[i] + b->coefficients_[i];
  }
  Expr new_rhs = SimplifiedAddition(a->rhs_, b->rhs_);
  return MatrixRow::make(new_coefficients, new_rhs);
}

RefMatrixRow operator-(RefMatrixRow a, RefMatrixRow b) {
  LOG(DEBUG) << COND(a->n_cols_ == b->n_cols_) << "Mismatch column number"
             << std::endl;
  vector<int> new_coefficients(a->n_cols_);
  for (size_t i = 0; i < a->n_cols_; i++) {
    new_coefficients[i] = a->coefficients_[i] - b->coefficients_[i];
  }
  Expr new_rhs = SimplifiedSubtraction(a->rhs_, b->rhs_);
  return MatrixRow::make(new_coefficients, new_rhs);
}
RefMatrixRow operator-(RefMatrixRow a) {
  vector<int> new_coefficients(a->n_cols_);
  for (size_t i = 0; i < a->n_cols_; i++) {
    new_coefficients[i] = -a->coefficients_[i];
  }
  Expr new_rhs = SimplifiedNegation(a->rhs_);
  return MatrixRow::make(new_coefficients, new_rhs);
}

RefMatrixRow operator*(RefMatrixRow a, int32_t scale) {
  vector<int> new_coefficients(a->n_cols_);
  for (size_t i = 0; i < a->n_cols_; i++) {
    new_coefficients[i] = a->coefficients_[i] * scale;
  }
  Expr new_rhs = SimplifiedMultiplication(a->rhs_, Expr(scale));
  return MatrixRow::make(new_coefficients, new_rhs);
}

int MatrixRow::get(size_t i) const{
  LOG(ERROR) << COND(i >= n_cols_) << "MatrixRow: Index " << i << i
             << " is out of vector range [0, " << n_cols_ << ") " << std::endl;
  return coefficients_[i];
}

RefMatrix Matrix::make(const vector<vector<int>> &all_coefficients,
                       vector<Expr> all_rhs) {
  LOG(ERROR) << COND(all_rhs.size() != all_coefficients.size())
             << "Mismatch row numbers in matrix" << std::endl;
  auto n_rows = all_rhs.size();
  vector<RefMatrixRow> rows{};
  for (size_t i = 0; i < n_rows; i++) {
    rows.push_back(MatrixRow::make(all_coefficients[i], all_rhs[i]));
  }
  return std::make_shared<Matrix>(rows);
}



ImplGaussianEliminationMethod::PackedResult
ImplGaussianEliminationMethod::operator()(RefMatrix &matrix,
                                          vector<Expr> &indexes) {
  int n_rows = matrix->n_rows_;
  int n_cols = matrix->n_cols_;
  vector<RefMatrixRow> &rows = matrix->rows_;
  vector<Expr> solutions(n_cols);
  vector<Expr> constraints;
  vector<bool> is_independent_variable(n_cols, false);
  vector<int> main_row(n_cols, 0);

  int current_row = 0;
  int last_main_col = -1;
  for(int i = 0; i < n_cols; i++){
    int non_zero_row_index = current_row;
    for(int j = current_row; j < n_rows; j++) if(rows[j]->get(i) != 0)
      { non_zero_row_index = j; break; }
    if(non_zero_row_index != current_row){
      std::swap(rows[current_row], rows[non_zero_row_index]);
    }
    if(rows[current_row]->get(i) == 0){
      // independent_variable
      is_independent_variable[i] = true;
      continue;
    }
    last_main_col = i;
    main_row[i] = current_row;
    if(rows[current_row]->get(i) < 0){
      rows[current_row] = - rows[current_row];
    }
    if(rows[current_row]->get(i) == 1){
      for(int j  = 0; j < n_rows; j++){
        if (j != current_row) {
          int32_t t = rows[j]->get(i);
          rows[j] = rows[j] - rows[current_row] * t;
        }
      }
    }else{
       for(int j = current_row + 1; j < n_rows; j++){
         while(rows[j]->get(i) != 0){
            int32_t t = rows[current_row]->get(i) / rows[j]->get(i);
            rows[current_row] = rows[current_row] - rows[j] * t;
            std::swap(rows[current_row], rows[j]);
         }
       }
    }
    current_row ++;
  }

  // zero row
  for(int j = current_row; j < n_rows; j++){
    auto compare_type = Type::int_scalar(32);
    constraints.push_back(Compare::make(compare_type, CompareOpType::EQ,
      rows[j]->rhs_, Expr(int32_t(0))));
  }

  // free variables
  for(int i = last_main_col + 1; i < n_cols; i++){
    auto index = indexes[i].as<Index>();
    auto index_type = Type::int_scalar(32);
    solutions[i] = Index::make(index_type, index->name, index->dom, IndexType::Reduce);
  }

  for(int i = last_main_col ; i >=0 ; i--) if (!is_independent_variable[i]){
    auto p = main_row[i];
    Expr e = rows[p]->rhs_;
    for(int k = i + 1; i < n_cols; i++) if(rows[p]->get(k)!=0){
        LOG(DEBUG) << COND(!solutions[k].defined()) << "Error: Gaussian Elimination Method" << std::endl;
        auto temp = SimplifiedMultiplication(solutions[k], Expr(rows[p]->get(k)));
        e = SimplifiedSubtraction(e, temp);
      }
    e = SimplifiedDivision(e, Expr(int32_t(rows[p]->get(i))));
    solutions[i] = e;
    auto dom = (indexes[i].as<Index>()->dom).as<Dom>();
    auto begin = dom->begin;
    auto end = SimplifiedAddition(dom->begin, dom->extent);
    auto compare_type = Type::int_scalar(32);
    constraints.push_back(Compare::make(compare_type, CompareOpType::LE, begin, e));
    constraints.push_back(Compare::make(compare_type, CompareOpType::LT, e, end));
  }

  return PackedResult{solutions, constraints};
}

}

}