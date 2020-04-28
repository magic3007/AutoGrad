#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <tuple>
#include "nlohmann/json.hpp"
#include "parser.h"
#include "CPPPrinter.h"

using namespace Boost::Internal;

using std::string;
using std::list;
using std::tuple;
using json = nlohmann::json;

#define LIST_OF_TESTCASES \
  X(example) \
  X(case1) \
  X(case4) \
  X(case5) \
  X(case6) \
  X(case7) \
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
    std::ifstream in_stream(input_file);
    if (!in_stream.is_open()){
      std::cerr << "Could not open file " << input_file << std::endl;
      exit(-1);
    }
    json j;
    in_stream >> j;
    in_stream.close();
    auto name = j["name"].get<string>();
    auto text = j["kernel"].get<string>();
    fprintf(stdout, "=========================================\n");
    fprintf(stdout, "%s: %s\n", name.c_str(), text.c_str());
    Group kernel = parser::ParseFromString(text, 0);
    CPPPrinter printer;
    std::cout << printer.print(kernel);
  }
  return 0;
}