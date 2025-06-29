# SyLife Logger Benchmarks
cmake_minimum_required(VERSION 3.20)

# Find Google Benchmark (optional)
find_package(benchmark QUIET)

if(benchmark_FOUND)
    # Benchmark executable
    add_executable(sylife_logger_benchmarks
        benchmark_logger.cpp
    )
    
    target_link_libraries(sylife_logger_benchmarks PRIVATE
        sylife_logger
        benchmark::benchmark
        benchmark::benchmark_main
    )
    
    target_compile_features(sylife_logger_benchmarks PRIVATE cxx_std_20)
    
    # Custom benchmark target
    add_custom_target(benchmark_logger
        COMMAND sylife_logger_benchmarks
        DEPENDS sylife_logger_benchmarks
        COMMENT "Running logger benchmarks"
    )
else()
    message(STATUS "Google Benchmark not found - skipping logger benchmarks")
    
    # Simple performance test without benchmark library
    add_executable(sylife_logger_perf_test
        simple_perf_test.cpp
    )
    
    target_link_libraries(sylife_logger_perf_test PRIVATE
        sylife_logger
    )
    
    target_compile_features(sylife_logger_perf_test PRIVATE cxx_std_20)
endif()