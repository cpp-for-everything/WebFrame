cmake_minimum_required(VERSION 3.14)

project("WebFrame++" VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(WebFrame_Library STATIC)

if(MSVC)
    target_compile_options(WebFrame_Library PRIVATE /W4 /await)
    target_compile_definitions(WebFrame_Library PRIVATE -DNOMINMAX)
    list(APPEND DEPENDANCES_WARNING_LEVEL "/W0")
else()
    # lots of warnings
    if(NOT(CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
        target_compile_options(WebFrame_Library PRIVATE -fcoroutines)
    endif()

    target_compile_options(WebFrame_Library PRIVATE -Wall -Wextra -Werror -pedantic)
    list(APPEND DEPENDANCES_WARNING_LEVEL "-w")
endif()

option(WITH_INJA "Use Inja" ON)
option(OPTIMIZATION "Compiler optimization level")
option(RUN_TESTS "Run unit & integration tests" ON)
option(BENCHMARKS "Compile for benchmarks" ON)

# Multithreading libraries
find_package(Threads REQUIRED)
target_link_libraries(WebFrame_Library PRIVATE ${CMAKE_THREAD_LIBS_INIT})

if(CMAKE_USE_PTHREADS_INIT)
    target_compile_options(WebFrame_Library PRIVATE -DUSE_PTHREAD)
endif()

# Versioning header
configure_file(include/webframe-version.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/webframe-version.hpp)

# Loading dependencies
add_subdirectory(third-party)

# Library sources
target_sources(WebFrame_Library PUBLIC src/webframe.cpp)
target_include_directories(WebFrame_Library
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/private
    ${CMAKE_BINARY_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/public
)

if(OPTIMIZATION)
    target_compile_options(WebFrame_Library ${OPTIMIZATION})
endif()

if(RUN_TESTS OR BENCHMARKS)
    include(CTest)
    enable_testing()
    add_subdirectory("tests/")
endif()

add_executable(WebFrame_Example_Sample example/Sample/main.cpp)
target_link_libraries(WebFrame_Example_Sample PUBLIC WebFrame_Library)
target_include_directories(WebFrame_Example_Sample PUBLIC
    "${PROJECT_BINARY_DIR}"
)

add_executable(WebFrame_Example_Project example/Project/main.cpp)
target_link_libraries(WebFrame_Example_Project PUBLIC WebFrame_Library)
target_include_directories(WebFrame_Example_Project PUBLIC
    "${PROJECT_BINARY_DIR}"
)

include(CPack)
