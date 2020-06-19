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

#include "gtest/gtest.h"
#include "parser.h"
#include "IR.h"
#include "IRPolisher.h"
#include "IndexAnalyst.h"
#include "IRPrinter.h"
#include "AutoDiffer.h"
#include "IRcppPrinter.h"
#include "utils/aixlog.hpp"


namespace {

using namespace Boost::Internal;

void foo(const string &text, const string &grad_to_str){
   AutoDiffer auto_differ;
   IRPrinter printer;
   IndexAnalyst index_analyzer;
   IRPolisher polisher;
   IRcppPrinter cpp_printer;

   auto main_stmt = parser::ParseFromString(text, 0).as<Kernel>()
        ->stmt_list[0];
   auto domains = index_analyzer(main_stmt);
   main_stmt = polisher(main_stmt, domains);

   std::cout << "===============================" << std::endl;
   std::cout << text << std::endl;
   std::cout << printer.print(main_stmt) << std::endl;
   std::cout << "grad_to : " << grad_to_str << std::endl;
   std::cout << std::endl;

   auto lhs = main_stmt.as<Move>()->dst.as<Var>();
   auto differential = Var::make(lhs->type(), "d" + lhs->name, lhs->args, lhs->shape);
   auto rv = auto_differ(main_stmt, grad_to_str);

   std::cout << printer.print(rv) << std::endl;

   std::cout << "Generated C code : " << std::endl;
   std::cout << cpp_printer.print(rv) << std::endl;
}

TEST(AutoDifferTest, Case1){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text = "C<4, 16>[i, j] = A<4, 16>[i, j] * B<4, 16>[i, j] + 1.0;";
  foo(text, "A");
  foo(text, "B");
}

TEST(AutoDifferTest, Case2){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text = "B<4, 16>[i, j] = A<4, 16>[i, j] * A<4, 16>[i, j] + 1.0;";
  foo(text, "A");
}

TEST(AutoDifferTest, Case3){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text = "C<4, 16>[i, j] = A<4, 16>[i, k] * B<16, 16>[k, j];";
  foo(text, "A");
  foo(text, "B");
}

TEST(AutoDifferTest, Case4){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text = "A<16, 32>[i, j] = B<16, 32>[i, k] * C<32, 32>[k, j];";
  foo(text, "B");
  foo(text, "C");
}

TEST(AutoDifferTest, Case5){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text = "A<16, 32>[i, j] =  B<16, 32, 4>[i, k, l] * C<32, 32>[k, j] * D<4, 32>[l, j];";
  foo(text, "B");
  foo(text, "C");
  foo(text, "D");
}

TEST(AutoDifferTest, Case6){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text = "A<2, 8, 5, 5>[n, k, p, q] = B<2, 16, 7, 7>[n, c, p + r, q + s] * C<8, 16, 3, 3>[k, c, r, s];";
  foo(text, "B");
  foo(text, "C");
}

TEST(AutoDifferTest, Case7){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text = "B<16, 32>[i, j] = A<32, 16>[j, i];";
  foo(text, "A");
}

TEST(AutoDifferTest, Case9){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text =  "B<4, 6>[i, j] = A<4>[i];";
  foo(text, "A");
}

TEST(AutoDifferTest, Case10){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  string text =  "A<8, 8>[i, j] = (B<10, 8>[i, j] + B<10, 8>[i + 1, j] + B<10, 8>[i + 2, j]) / 3.0;";
  foo(text, "B");
}
}