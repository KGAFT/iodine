project(iodine-contrib)

file(GLOB_RECURSE CNTR_FILES src/*.*)

add_library(iodine-contrib STATIC ${CNTR_FILES})

target_include_directories(iodine-contrib PUBLIC src/)