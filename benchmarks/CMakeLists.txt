include_directories(../src/)

function(add_benchmark name)
  add_executable(
    ${name}
    ${name}.cxx
  )
  target_link_libraries(
    ${name}
    PRIVATE
    PoolStrategy
    benchmark::benchmark
  )
endfunction()

add_benchmark(ArbitrageStrategyBenchmark)