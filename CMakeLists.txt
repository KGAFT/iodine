cmake_minimum_required(VERSION 3.9)

project(iodine)

file(GLOB_RECURSE SRC_FILES src/*.*)

add_library(iodine-static STATIC ${SRC_FILES})
add_library(iodine-shared SHARED ${SRC_FILES})

target_include_directories(iodine-static PUBLIC src/)
target_include_directories(iodine-shared PUBLIC src/)

file(GLOB_RECURSE TEST_FILES test/*.*)

add_executable(iodine-testing ${TEST_FILES})
target_link_libraries(iodine-testing PUBLIC iodine-static)
target_include_directories(iodine-testing PUBLIC test/)
