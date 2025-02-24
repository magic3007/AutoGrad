find_package(BISON)
find_package(FLEX)

# reuseable cmake macro for yacc
macro(yacc_config target_name _filename)
    get_filename_component(_basename ${_filename} NAME_WE)
    BISON_TARGET(${target_name} ${_filename} ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h
        REPORT_FILE  ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.report
    )
ENDMACRO()

# reuseable cmake macro for lex
MACRO(lex_config target_name _filename)
    GET_FILENAME_COMPONENT(_basename ${_filename} NAME_WE)
    FLEX_TARGET(${target_name} ${_filename} ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.cc
            DEFINES_FILE  ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h)
ENDMACRO()

# Another script: Credit to https://cmake.org/pipermail/cmake/2007-July/015398.html

yacc_config(BisonTarget grammar.y)
lex_config(FlexTarget token.l)
ADD_FLEX_BISON_DEPENDENCY(FlexTarget BisonTarget)

add_library(parser  STATIC
        parser.h  ${FLEX_FlexTarget_OUTPUTS} ${BISON_BisonTarget_OUTPUTS}
        )
add_library_config(parser)
target_link_libraries(parser ${LIB_NAME})

include_directories(3rd)

### test_main

add_executable(parser_main
        test_main/parser_main.cc parser.h
)
add_executable_config(parser_main)
link_generated_library(parser_main parser)

add_executable(test_cpp_printer
        test_main/test_cpp_printer.cc CPPPrinter.cc parser.h
)
add_executable_config(test_cpp_printer)
link_generated_library(test_cpp_printer parser)