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

# =====================================================================================

function(add_executable_config TARGETNAME)
    # TODO: consider check_and_add_compiler_option()
    target_compile_options(${TARGETNAME} PRIVATE
        -Wall -Wextra -Werror
            -Wno-unused-parameter
            -Wno-unused-function
        )
#    Cmake 3.17 or later
#    target_link_directories(${TARGETNAME}
#            PRIVATE "${PROJECT_BINARY_DIR}/lib"
#            )
    set_target_properties(${TARGETNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
            "${PROJECT_BINARY_DIR}/bin")
endfunction()

function(add_library_config TARGETNAME)
    # TODO: consider check_and_add_compiler_option()
    target_compile_options(${TARGETNAME} PRIVATE
        -Wall -Wextra -Werror
            -Wno-unused-parameter
            -Wno-unused-function
        )
#    Cmake 3.17 or later
#    target_link_directories(${TARGETNAME}
#            PRIVATE "${PROJECT_BINARY_DIR}/lib"
#            )
    set_target_properties(${TARGETNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
            "${PROJECT_BINARY_DIR}/lib")
endfunction()

set(defualt_generated_library_path "${PROJECT_BINARY_DIR}/lib")

function(link_generated_library TARGETNAME LIBNAME)
    include_directories(${defualt_generated_library_path})
    target_link_libraries(${TARGETNAME} PRIVATE ${LIBNAME})
endfunction()

# ===============================================================================

add_subdirectory(utils)

yacc_config(BisonTarget grammar.y)
lex_config(FlexTarget token.l)
ADD_FLEX_BISON_DEPENDENCY(FlexTarget BisonTarget)

add_library(parser  STATIC
        parser.h  ${FLEX_FlexTarget_OUTPUTS} ${BISON_BisonTarget_OUTPUTS}
        )
add_library_config(parser)
target_link_libraries(parser ${LIB_NAME})

### test_main

add_executable(parser_main
        test_main/parser_main.cc parser.h
)
add_executable_config(parser_main)
link_generated_library(parser_main parser)