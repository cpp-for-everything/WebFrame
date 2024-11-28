cmake_minimum_required(VERSION 3.14)

if(RUN_TESTS)
    add_executable(WebFrame_IntegrationTests integration_tests/main.cpp)
    target_include_directories(WebFrame_IntegrationTests PUBLIC "${PROJECT_SOURCE_DIR}/tests")
    target_link_libraries(WebFrame_IntegrationTests PUBLIC WebFrame_Library Catch2::Catch2)

    add_test(NAME WebFrame_IntegrationTests COMMAND WebFrame_IntegrationTests)
endif()

if(BENCHMARKS)
    # Generate example source code
    add_executable(WebFrame_Benchmark_Volatile benchmark/server_volatile.cpp)
    target_link_libraries(WebFrame_Benchmark_Volatile PUBLIC WebFrame_Library)
    target_include_directories(WebFrame_Benchmark_Volatile PUBLIC
        "${PROJECT_BINARY_DIR}"
    )

    add_executable(WebFrame_Benchmark_Atomic benchmark/server_atomic.cpp)
    target_link_libraries(WebFrame_Benchmark_Atomic PUBLIC WebFrame_Library)
    target_include_directories(WebFrame_Benchmark_Atomic PUBLIC
        "${PROJECT_BINARY_DIR}"
    )
endif()