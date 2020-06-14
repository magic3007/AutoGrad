file(GLOB test_src "*.cc")
file(GLOB kernel_src "./kernels/*.cc")
file(GLOB solution_src
        ./solution/solution2.cc)
file(GLOB clean_src "./clean/*.cc")

include_directories("../include")

add_subdirectory(solution)

add_executable(test2 ${test_src} ${kernel_src})
add_executable(solution2 ${solution_src})
target_include_directories(solution2 PRIVATE
        ./solutions/3rd
        ${PROJECT_SOURCE_DIR}/project1/solutions
        ${PROJECT_SOURCE_DIR}/project1/solutions/3rd)
target_link_libraries(solution2 ${LIB_NAME} parser)
add_executable(cleanf2 ${clean_src})

add_custom_target(solution_run2
  COMMAND solution2
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "run generated solution in ${CMAKE_CURRENT_SOURCE_DIR}"
  SOURCES ${solution_src}
)

add_custom_target(clean_run2
  COMMAND cleanf2
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "run generated clean in ${CMAKE_CURRENT_SOURCE_DIR}"
  SOURCES ${clean_src}
)

add_dependencies(solution_run2 clean_run2)
add_dependencies(test2 solution_run2)