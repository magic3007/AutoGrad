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

#ifndef BOOST_ARITH_H
#define BOOST_ARITH_H

#include "IR.h"
#include "utils/base.h"
#include <vector>
#include <string>

namespace arithmetic {

namespace gaussian_elimination {

using namespace Boost::Internal;
using std::vector;
using std::string;

/* forward declaration */
class MatrixRow;
class Matrix;
class ImplGaussianEliminationMethod;

typedef Ref<MatrixRow> RefMatrixRow;
typedef Ref<Matrix> RefMatrix;

class MatrixRow{
  DISALLOW_COPY_AND_ASSIGN(MatrixRow);
  public:
    explicit MatrixRow(vector<int> coefficients, const Expr &rhs)
      : n_cols_(coefficients.size()),
        coefficients_(std::move(coefficients)),
        rhs_(rhs){}
    static RefMatrixRow make(const vector<int> &coefficients, const Expr &rhs);

    int get(size_t i) const ;
    vector<int> get_coefficients(){return coefficients_;}
    Expr get_rhs() {return rhs_;}
    friend RefMatrixRow operator+(RefMatrixRow a, RefMatrixRow b);
    friend RefMatrixRow operator-(RefMatrixRow a, RefMatrixRow b);
    friend RefMatrixRow operator-(RefMatrixRow a);
    friend RefMatrixRow operator*(RefMatrixRow a, int32_t scale);

    friend Matrix;
    friend ImplGaussianEliminationMethod;

  private:
    size_t n_cols_;
    vector<int> coefficients_;
    Expr rhs_;
};

class Matrix{
  DISALLOW_COPY_AND_ASSIGN(Matrix);
  public:
    explicit Matrix(vector<RefMatrixRow> rows)
      : n_rows_(rows.size()), n_cols_(rows[0]->n_cols_),
        rows_(std::move(rows)){}
    static RefMatrix make(const vector<vector<int>> &coefficients, vector<Expr> all_rhs);
    friend ImplGaussianEliminationMethod;
    size_t get_n_rows() const {return n_rows_;}
    size_t get_n_cols() const {return n_cols_;}
    vector<RefMatrixRow> get_rows(){ return rows_; }
  private:
    size_t n_rows_, n_cols_;
    vector<RefMatrixRow> rows_;
};

class ImplGaussianEliminationMethod{
  DISALLOW_COPY_AND_ASSIGN(ImplGaussianEliminationMethod);
  public:
    explicit ImplGaussianEliminationMethod()= default;
    class PackedResult{
    public:
      vector<Expr> solutions;
      vector<Expr> constraints;
    };

    /**
     *
     * @param matrix The system of linear equations to be solved. Note that this
     * parameter is passed by reference, and this functor will modified |matrix|
     * in place, so remember to backup |matrix| if any use later.
     * @param indexes |n_cols_| original index variables
     * @return This functor will return a packed result by Class |PackedResult|.
     * Support that are |n_cols_| columns, namely |n_cols_| variables in this
     * system of linear equations. |PackedResult.solutions| will contain |n_cols_|
     * expressions, in which the i-th one is the solution to the i-th variable.
     * Notice that we may create some free variables in the solutions.
     * Meanwhile, in order to ensure that the solution exists, the right-hand-side
     * of zero rows should be equal to zero. Such constraints are store in
     * |PackedResult.constraints|.
     */
    PackedResult operator()(RefMatrix &matrix, vector<Expr> &indexes);

};

}

}


#endif // BOOST_ARITH_H
