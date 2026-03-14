cmake_minimum_required(VERSION 3.9)

project(iodine)

file(GLOB_RECURSE SRC_FILES src/*.*)

add_subdirectory(contrib)

add_library(iodine-static STATIC ${SRC_FILES})
add_library(iodine-shared SHARED ${SRC_FILES})

target_include_directories(iodine-static PUBLIC src/)
target_include_directories(iodine-shared PUBLIC src/)

target_compile_options(iodine-static PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
target_compile_options(iodine-shared PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_libraries(iodine-static PRIVATE iodine-contrib)
target_link_libraries(iodine-shared PRIVATE iodine-contrib)

file(GLOB_RECURSE TEST_FILES test/*.*)

add_executable(iodine-testing ${TEST_FILES})
target_link_libraries(iodine-testing PUBLIC iodine-static)
target_include_directories(iodine-testing PUBLIC test/)
