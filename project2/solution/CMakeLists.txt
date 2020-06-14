include_directories(${PROJECT_SOURCE_DIR}/project1/solutions)
include_directories(${PROJECT_SOURCE_DIR}/project1/solutions/3rd)
include_directories(3rd)

add_library(autodiff  STATIC
        IndexAnalyst.cpp
        IRPolisher.cpp
        )
add_library_config(autodiff)
target_link_libraries(autodiff ${LIB_NAME})

# =========================================

add_subdirectory(Google_tests)