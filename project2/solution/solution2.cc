#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <string>
#include <tuple>
#include "nlohmann/json.hpp"
#include "utils/base.h"
#include "parser.h"
#include "signPrinter2.h"

using namespace Boost::Internal;

using std::string;
using std::list;
using std::tuple;
using json = nlohmann::json;

#define LIST_OF_TESTCASES \
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
      #define X(name) {"./cases/" #name ".json","./kernels/grad_" #name ".cc"},
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
    auto grad_to = j["grad_to"];
    signPrinter2 sprinter2(ins, outs, type, grad_to);
    Group kernel = parser::ParseFromString(text, 0);
    std::cerr << "Generate successfully!" << std::endl;
    
    std::cout << "void " << name.c_str() << sprinter2.print(kernel) << std::endl;
  }
  return 0;
}
