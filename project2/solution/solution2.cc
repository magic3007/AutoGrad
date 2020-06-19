#include "nlohmann/json.hpp"
#include "parser.h"
#include "signPrinter2.h"
#include "utils/base.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <tuple>

#include "AutoDiffer.h"
#include "IR.h"
#include "IRPolisher.h"
#include "IndexAnalyst.h"
#include "IRcppPrinter.h"
#include "utils/aixlog.hpp"

using namespace Boost::Internal;

using std::list;
using std::string;
using std::tuple;
using json = nlohmann::json;

#define LIST_OF_TESTCASES                                                      \
  X(case1)                                                                     \
  X(case2)                                                                     \
  X(case3)                                                                     \
  X(case4)                                                                     \
  X(case5)                                                                     \
  X(case6)                                                                     \
  X(case7)                                                                     \
  X(case9)                                                                     \
  X(case10)

Group foo(const string &text, const string &grad_to_str) {
  AutoDiffer auto_differ;
  IRPrinter printer;
  IndexAnalyst index_analyzer;
  IRPolisher polisher;

  auto main_stmt = parser::ParseFromString(text, 0).as<Kernel>()->stmt_list[0];
  auto domains = index_analyzer(main_stmt);
  main_stmt = polisher(main_stmt, domains);
  auto lhs = main_stmt.as<Move>()->dst.as<Var>();
  auto differential = Var::make(lhs->type(), "d" + lhs->name, lhs->args, lhs->shape);
  return auto_differ(main_stmt, grad_to_str);
}

std::string generate_sign(const string &text, std::vector<std::string> grad,
                          std::vector<std::string> _ins,
                          std::vector<std::string> _outs, std::string _type);

int main(int argc, char *argv[]) {
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace,
                                      AixLog::Type::normal);
  list<tuple<string, string>> testcases = {
#define X(name) {"./cases/" #name ".json", "./kernels/grad_" #name ".cc"},
      LIST_OF_TESTCASES
#undef X
  };
  for (auto &x : testcases) {
    auto input_file = std::get<0>(x);
    auto output_file = std::get<1>(x);
    std::cerr << "[" << std::left << std::setw(30) << input_file << "]: ";
    std::ifstream in_stream(input_file);
    if (!in_stream.is_open()) {
      std::cerr << "Could not open file " << input_file << std::endl;
      continue;
    }
    DEFER({ in_stream.close(); });
    std::ofstream out_stream(output_file);
    if (!out_stream.is_open()) {
      std::cerr << "Could not open file " << output_file << std::endl;
      continue;
    }
    DEFER({ out_stream.close(); });
    json j;
    in_stream >> j;
    auto name = j["name"].get<string>();
    auto text = j["kernel"].get<string>();
    auto ins = j["ins"];
    auto outs = j["outs"];
    auto type = j["data_type"].get<string>();
    auto grad_to = j["grad_to"];
    string body_stmt = "";
    IRcppPrinter printer;
    for(const auto &grad : grad_to){
      auto rv = foo(text, grad);
      body_stmt += printer.print(rv);
    }
    out_stream << "void " << name.c_str()
               << generate_sign(text, grad_to, ins, outs, type) << "{"
               << std::endl;
    out_stream << body_stmt << "\n}" << std::endl;
    std::cerr << "Generate successfully!" << std::endl;
  }
  return 0;
}

std::string generate_sign(const string &text, std::vector<std::string> grad,
                          std::vector<std::string> ins,
                          std::vector<std::string> outs, std::string type) {
  AutoDiffer auto_differ;
  IRPrinter printer;
  IndexAnalyst index_analyzer;
  IRPolisher polisher;

  auto main_stmt = parser::ParseFromString(text, 0).as<Kernel>()->stmt_list[0];
  auto domains = index_analyzer(main_stmt);
  main_stmt = polisher(main_stmt, domains);
  signPrinter2 sprinter2;
  std::map<std::string, std::string> range;
  range.clear();
  std::map<std::string, std::string>::iterator it;
  for (size_t i = 0; i < grad.size(); ++i) {

    auto lhs = main_stmt.as<Move>()->dst.as<Var>();
    auto differential =
        Var::make(lhs->type(), "d" + lhs->name, lhs->args, lhs->shape);
    auto rv = auto_differ(main_stmt, grad[i]);
    std::map<std::string, std::string> crange = sprinter2.get(rv);
    for (it = crange.begin(); it != crange.end(); it++)
      range[it->first] = it->second;
  }

  std::string ret = "(";
  bool first = 1;
  for (size_t i = 0; i < ins.size(); ++i) {
    std::string name = ins[i];
    if (range.find(name) == range.end())
      continue;
    std::string size = range[name];
    if (!first)
      ret += ", ";
    ret += type + " ";
    first = 0;
    if (size.length() == 0)
      ret += "&" + name;
    else
      ret += "(&" + name + ")" + size;
  }
  for (size_t i = 0; i < outs.size(); ++i) {
    std::string name = "d" + outs[i];
    if (range.find(name) == range.end())
      continue;
    std::string size = range[name];
    if (!first)
      ret += ", ";
    ret += type + " ";
    first = 0;
    if (size.length() == 0)
      ret += "&" + name;
    else
      ret += "(&" + name + ")" + size;
  }
  for (size_t i = 0; i < grad.size(); ++i) {
    std::string name = "d" + grad[i];
    if (range.find(name) == range.end())
      continue;
    std::string size = range[name];
    if (!first)
      ret += ", ";
    ret += type + " ";
    first = 0;
    if (size.length() == 0)
      ret += "&" + name;
    else
      ret += "(&" + name + ")" + size;
  }
  ret += ")";
  return ret;
}
