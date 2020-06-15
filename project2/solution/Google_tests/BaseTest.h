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

#ifndef BOOST_BASETEST_H
#define BOOST_BASETEST_H

#include "gtest/gtest.h"
#include "IR.h"
#include "parser.h"
#include <string>

using namespace Boost::Internal;
using ::testing::TestWithParam;
using ::testing::Values;
using std::string;


typedef string GetTextFunc();

class BaseTest : public TestWithParam<GetTextFunc*>{
protected:
  void SetUp() override{
    text_ = (*GetParam())();
    // support there is only one statement in input testcases.
    main_stmt_ = parser::ParseFromString(text_, 0).as<Kernel>()
        ->stmt_list[0];
  }

  Stmt main_stmt_;
  string text_;
};

string get_case1_text(){
  return "C<4, 16>[i, j] = A<4, 16>[i, j] * B<4, 16>[i, j] + 1.0;";
}

string get_case2_text(){
  return "B<4, 16>[i, j] = A<4, 16>[i, j] * A<4, 16>[i, j] + 1.0;";
}

string get_case3_text(){
  return "C<4, 16>[i, j] = A<4, 16>[i, k] * B<16, 16>[k, j];";
}

string get_case4_text(){
  return "A<16, 32>[i, j] = B<16, 32>[i, k] * C<32, 32>[k, j];";
}

string get_case5_text(){
  return "A<16, 32>[i, j] =  B<16, 32, 4>[i, k, l] * C<32, 32>[k, j] * D<4, 32>[l, j];";
}

string get_case6_text(){
  return "A<2, 8, 5, 5>[n, k, p, q] = B<2, 16, 7, 7>[n, c, p + r, q + s] * C<8, 16, 3, 3>[k, c, r, s];";
}

string get_case7_text(){
  return "B<16, 32>[i, j] = A<32, 16>[j, i];";
}

string get_case8_text(){
  return "B<32>[i] = A<2, 16>[i//16, i%16];";
}

string get_case9_text(){
  return "B<4, 6>[i, j] = A<4>[i];";
}

string get_case10_text(){
  return "A<8, 8>[i, j] = (B<10, 8>[i, j] + B<10, 8>[i + 1, j] + B<10, 8>[i + 2, j]) / 3.0;";
}

#endif // BOOST_BASETEST_H
