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
#include "gtest/gtest.h"
#include "utils/aixlog.hpp"
#include "IRPrinter.h"

namespace {

using namespace arithmetic::gaussian_elimination;
using std::string;

TEST(MatrixRow, DisplayMatirxRow) {
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace,
                                      AixLog::Type::all);

  Type index_type = Type::int_scalar(32);

  const int N = 256;
  const int K = 1024;

  // index n
  Expr dom_n = Dom::make(index_type, 0, N);
  Expr n = Index::make(index_type, "n", dom_n, IndexType::Spatial);
  // index k
  Expr dom_k = Dom::make(index_type, 0, K);
  Expr k = Index::make(index_type, "k", dom_k, IndexType::Spatial);

  auto coeff_1 = {0, 1, 2};
  auto coeff_2 = {2, 3, 4};

  auto row_1 = MatrixRow::make(coeff_1, n);
  auto row_2 = MatrixRow::make(coeff_2, k);

  auto print_vector = [](const vector<int> &vec) -> string {
    std::ostringstream oss;
    for (auto iter : vec)
      oss << iter << " ";
    return oss.str();
  };

  IRPrinter printer;

  {
    std::cout << "=====================================" << std::endl;
    std::cout << "Original:" << std::endl;
    std::cout << print_vector(coeff_1) << printer.print(n) << std::endl;
    std::cout << print_vector(coeff_2) << printer.print(k) << std::endl;
    std::cout << "After Addition: " << std::endl;
    auto temp = row_1 + row_2;
    std::cout << print_vector(temp->get_coefficients())
              << printer.print(temp->get_rhs()) << std::endl;
  }

  {
    std::cout << "=====================================" << std::endl;
    std::cout << "Original:" << std::endl;
    std::cout << print_vector(coeff_1) << printer.print(n) << std::endl;
    std::cout << print_vector(coeff_2) << printer.print(k) << std::endl;
    std::cout << "After Subtraction: " << std::endl;
    auto temp = row_1 - row_2;
    std::cout << print_vector(temp->get_coefficients())
              << printer.print(temp->get_rhs()) << std::endl;
  }

  {
    std::cout << "=====================================" << std::endl;
    std::cout << "Original:" << std::endl;
    std::cout << print_vector(coeff_1) << printer.print(n) << std::endl;
    std::cout << "After Negation: " << std::endl;
    auto temp = -row_1;
    std::cout << print_vector(temp->get_coefficients())
              << printer.print(temp->get_rhs()) << std::endl;
  }

  {
    std::cout << "=====================================" << std::endl;
    int32_t scale = 119;
    std::cout << "Original:" << std::endl;
    std::cout << print_vector(coeff_1) << printer.print(n) << std::endl;
    std::cout << "scale : " << scale << std::endl;
    std::cout << "After Scaling: " << std::endl;
    auto temp = row_1 * scale;
    std::cout << print_vector(temp->get_coefficients())
              << printer.print(temp->get_rhs()) << std::endl;
  }
}

class ImplGaussianEliminationMethodTest : public testing::Test {
protected:
  void SetUp() override {
    const int N = 256;
    const int C = 1024;
    const int P = 7;
    const int Q = 7;
    const int H = 9;
    const int W = 9;
    const int K = 1024;
    const int R = 3;
    const int S = 3;
    Type index_type = Type::int_scalar(32);
    Type data_type = Type::float_scalar(32);

    // index n
    Expr dom_n = Dom::make(index_type, 0, N);
    n = Index::make(index_type, "n", dom_n, IndexType::Spatial);

    // index k
    Expr dom_k = Dom::make(index_type, 0, K);
    k = Index::make(index_type, "k", dom_k, IndexType::Spatial);

    // index p
    Expr dom_p = Dom::make(index_type, 0, P);
    p = Index::make(index_type, "p", dom_p, IndexType::Spatial);

    // index
    Expr dom_q = Dom::make(index_type, 0, Q);
    q = Index::make(index_type, "q", dom_q, IndexType::Spatial);

    // index c
    Expr dom_c = Dom::make(index_type, 0, C);
    c = Index::make(index_type, "c", dom_c, IndexType::Reduce);

    // index h
    Expr dom_h = Dom::make(index_type, 0, H);
    h = Index::make(index_type, "h", dom_h, IndexType::Spatial);

    // index w
    Expr dom_w = Dom::make(index_type, 0, W);
    w = Index::make(index_type, "w", dom_w, IndexType::Spatial);

    // index r
    Expr dom_r = Dom::make(index_type, 0, R);
    r = Index::make(index_type, "r", dom_r, IndexType::Reduce);

    // index s
    Expr dom_s = Dom::make(index_type, 0, S);
    s = Index::make(index_type, "s", dom_s, IndexType::Reduce);
  }

  Expr n, k, p, q, c, h, w, r, s;

  static string print_vector(const vector<int> &vec) {
    std::ostringstream oss;
    for (auto iter : vec)
      oss << iter << " ";
    return oss.str();
  }

  static string print_expr_vector(const vector<Expr> &vec) {
    std::ostringstream oss;
    IRPrinter printer;
    for (const auto& iter : vec)
      oss << printer.print(iter) << std::endl;
    return oss.str();
  }

  static string print_matrix(RefMatrix matrix) {
    std::ostringstream oss;
    auto n_rows = matrix->get_n_rows();
    auto n_cols = matrix->get_n_cols();
    auto rows = matrix->get_rows();
    IRPrinter printer;
    for(int i = 0; i < n_rows; ++i) {
      oss << print_vector(rows[i]->get_coefficients())
        << printer.print(rows[i]->get_rhs()) << std::endl;
    }
    return oss.str();
  }

  static string print_index_vector(vector<Expr> &indexes){
    std::ostringstream oss;
    IRPrinter printer;
    printer.set_print_range(true);
    for (const auto& iter : indexes){
      oss << printer.print(iter) << std::endl;
    }
    return oss.str();
  }

  void solve(const vector<vector<int>> &coeff, vector<Expr> &indexes, vector<Expr> &rhs){
    auto matrix = Matrix::make(coeff, rhs);
    ImplGaussianEliminationMethod impl;

    std::cout << "Original:" << std::endl;
    std::cout << print_matrix(matrix) << std::endl;
    std::cout << print_index_vector(indexes) << std::endl;
    using PackedResult = ImplGaussianEliminationMethod::PackedResult;
    PackedResult rv = impl(matrix, indexes);
    std::cout << "After Gaussian Elimination Method : " << std::endl;
    std::cout << print_matrix(matrix) << std::endl;
    std::cout << "solutions: " << std::endl;
    std::cout << print_expr_vector(rv.solutions) << std::endl;
    std::cout << "constraints: " << std::endl;
    std::cout << print_expr_vector(rv.constraints) << std::endl;
  }
};

TEST_F(ImplGaussianEliminationMethodTest, DisplayNormalSituation) {
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::debug,
                                      AixLog::Type::all);
    vector<vector<int>> coeff = {
        {1,1,3},
        {2,4,1},
        {1,2,4},
    };
    vector<Expr> indexes = {w, r, s};
    vector<Expr> rhs =  {n, k, p};
    solve(coeff, indexes, rhs);
}

TEST_F(ImplGaussianEliminationMethodTest, DisplaySituationWithFreeVariableAndIndependentVariable) {
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::debug,
                                      AixLog::Type::all);
    vector<vector<int>> coeff = {
        {1,0,2,1},
        {1,0,2,3},
        {1,0,2,9},
    };
    vector<Expr> indexes = {w, r, s, n};
    vector<Expr> rhs =  {k, p, c};
    solve(coeff, indexes, rhs);
}
}