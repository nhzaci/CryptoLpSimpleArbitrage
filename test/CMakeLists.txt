# reference: https://google.github.io/googletest/quickstart-cmake.html

include(GoogleTest)
include_directories(../src/)

function(add_test name) 
  add_executable(
    ${name}
    ${name}.cxx
  )
  target_link_libraries(
    ${name}
    PoolStrategy
    GTest::gtest_main
  )
  gtest_discover_tests(${name})
endfunction()

add_test(TestMain)