cmake_minimum_required(VERSION 3.14)

project(param_vertex_cover LANGUAGES CXX)

set(default_build_type "Debug")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTS "Run tests" ON)
option(PROFILE "Compile with gprof for profiling" ON)

message(STATUS "Buildtype is ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra -Werror -g3")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O3")

if (BUILD_TESTS)
    message(STATUS "Building googletests")
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

set(tname param_vc)

add_executable(
    ${tname}
    src/main.cpp
)

if(PROFILE)
    message(STATUS "Building with gprof")
    target_compile_options(${tname} PUBLIC -pg)
    target_link_options(${tname} PUBLIC -pg)
endif()

target_link_libraries(
    ${tname}
    LINK_PUBLIC
    m
    pthread
)

if(BUILD_TESTS)
    enable_testing()
    add_executable(
        test_main
        src/tests/test.cpp
        src/main.cpp
    )
    target_compile_definitions(test_main PUBLIC WITH_GTESTS) 
    target_link_libraries(
        test_main
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(test_main)
endif()