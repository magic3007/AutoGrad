#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <string>
#include <tuple>
#include "nlohmann/json.hpp"
#include "utils/base.h"
#include "parser.h"
#include "CPPPrinter.h"
#include "signPrinter.h"

using namespace Boost::Internal;

using std::string;
using std::list;
using std::tuple;
using json = nlohmann::json;

#define LIST_OF_TESTCASES \
  X(example) \
  X(case1) \
  X(case2) \
  X(case3) \
  X(case4) \
  X(case5) \
  X(case6) \
  X(case7) \
  X(case8) \
  X(case9) \
  X(case10)

int main(int argc, char *argv[]){
  list<tuple<string, string>> testcases = {
      #define X(name) {"./cases/" #name ".json","./kernels/kernel_" #name ".cc"},
        LIST_OF_TESTCASES
      #undef X
  };
  for(auto &x : testcases){
    auto input_file = std::get<0>(x);
    auto output_file = std::get<1>(x);
    std::cerr << "[" << std::left << std::setw(30) << input_file  << "]: ";
    std::ifstream in_stream(input_file);
    if (!in_stream.is_open()){
      std::cerr << "Could not open file " << input_file << std::endl;
      continue;
    }
    DEFER({in_stream.close();});
    std::ofstream out_stream(output_file);
    if(!out_stream.is_open()){
      std::cerr << "Could not open file " << output_file << std::endl;
      continue;
    }
    DEFER({out_stream.close();});
    json j;
    in_stream >> j;
    auto name = j["name"].get<string>();
    auto text = j["kernel"].get<string>();
    auto ins = j["ins"];
    auto outs = j["outs"];
	  auto type = j["data_type"].get<string>();
    Group kernel = parser::ParseFromString(text, 0);
    signPrinter sprinter(ins, outs, type);
    out_stream << "void "<< name.c_str() << sprinter.print(kernel) << "{" << std::endl;
    CPPPrinter printer;
    out_stream << printer.print(kernel) << std::endl;
    out_stream << "}" << std::endl;
    std::cerr << "Generate successfully!" << std::endl;
  }
  return 0;
}
