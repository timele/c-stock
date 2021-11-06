cmake_minimum_required (VERSION 3.16)

# Setup testing
enable_testing()

# Copy data driven test files
add_custom_target(copy_test_data)
set(test_data
  ${PROJECT_SOURCE_DIR}/test/input.data
  ${PROJECT_SOURCE_DIR}/test/check.data
)
foreach(test_file_data ${test_data})
  add_custom_command(TARGET copy_test_data PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${test_file_data} /tmp/)
endforeach()

#------------------
# Sources
#------------------
include_directories("${PROJECT_SOURCE_DIR}/include")
file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/main.c")

#------------------
# Test Stock
#------------------
add_executable(TestStock StockTest.cpp ${sources})
add_dependencies(TestStock copy_test_data)
target_link_libraries(
        TestStock
        gtest
        gtest_main
)
add_test(TestStock TestStock)

#------------------
# Test Coverage
#------------------
include(../cmake/CodeCoverage.cmake)
APPEND_COVERAGE_COMPILER_FLAGS()
set(COVERAGE_EXCLUDES
  "/usr/include/*" "gtest/*"
  "${CMAKE_BINARY_DIR}/_deps/googletest-src/*"
)
setup_target_for_coverage_lcov(
  NAME coverage
  EXECUTABLE ctest -j ${n_cores}
  EXCLUDE ${COVERAGE_EXCLUDES}
  dependencies TestStock
)

