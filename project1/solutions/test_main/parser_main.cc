#include "../parser.h"
#include "IR.h"
#include "IRVisitor.h"
#include "IRMutator.h"
#include "IRPrinter.h"

using namespace Boost::Internal;

int main(int argc, char *argv[]){
  Group kernel = parser::ParseFromStdin();
  // printer
  IRPrinter printer;
  std::string code = printer.print(kernel);
  std::cout << code;
  return 0;
}