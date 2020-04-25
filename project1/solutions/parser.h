#ifndef BOOST_PARSER_H
#define BOOST_PARSER_H
#include <stdio.h>
#include "IR.h"

extern int yyparse();
extern void set_yyparse_file_ptr(FILE *file_ptr);
extern Boost::Internal::Group root;
extern int yydebug;

namespace parser{

using namespace Boost::Internal;
using std::string;

Group ParseFromStdin(){
  set_yyparse_file_ptr(stdin);
  yyparse();
  return root;
}

Group ParseFromFile(const char *filename){
  auto file_ptr = fopen(filename, "r");
  set_yyparse_file_ptr(file_ptr);
  yyparse();
  fclose(file_ptr);
  return root;
}

Group ParseFromString(const string &text, int verbose=0){
  auto file_ptr = tmpfile();
  fputs(text.c_str(), file_ptr);
  rewind(file_ptr);
  set_yyparse_file_ptr(file_ptr);
  yydebug = verbose;
  yyparse();
  fclose(file_ptr);
  return root;
}

}


#endif // BOOST_PARSER_H
