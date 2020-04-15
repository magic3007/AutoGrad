#ifndef BOOST_PARSER_H
#define BOOST_PARSER_H
#include <stdio.h>
#include "IR.h"

namespace parser{

extern Boost::Internal::Group root;

extern int yyparse();
extern void set_yyparse_file_ptr(FILE *file_ptr);

Boost::Internal::Group ParseFromStdin(){
  set_yyparse_file_ptr(stdin);
  yyparse();
  return root;
}

Boost::Internal::Group ParseFromFile(const char *filename){
  auto file_ptr = fopen(filename, "r");
  set_yyparse_file_ptr(file_ptr);
  yyparse();
  fclose(file_ptr);
  return root;
}

}


#endif // BOOST_PARSER_H
