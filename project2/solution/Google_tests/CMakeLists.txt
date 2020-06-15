project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# =====================================
# helper functions
# =====================================

function(link_google_test_library TARGETNAME)
    target_link_libraries(${TARGETNAME} PRIVATE gtest gtest_main)
endfunction()

# =====================================

include_directories(..)

add_executable(Google_Tests_test_polisher IRPolisherTest.cpp)
link_generated_library(Google_Tests_test_polisher parser)
link_generated_library(Google_Tests_test_polisher autodiff)
link_google_test_library(Google_Tests_test_polisher)