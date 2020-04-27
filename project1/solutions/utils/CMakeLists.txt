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

# =================================================================

add_library(utils STATIC
        argument_parser.cpp
        base_utils.h
        lock.h
        logging.cpp
        )
add_library_config(utils)