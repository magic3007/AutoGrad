#include <cstdio>
#include <iostream>
#include <list>
#include <string>
#include <tuple>

using std::string;
using std::list;
using std::tuple;

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
    std::cout<< input_file << " " << output_file << std::endl;
  }
  return 0;
}