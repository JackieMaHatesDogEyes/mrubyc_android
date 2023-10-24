cmake_minimum_required(VERSION 3.22.1)

project("mrubyc")


add_library(${CMAKE_PROJECT_NAME}
    # mruby files
    src/alloc.c
    src/c_array.c
    src/c_hash.c
    src/c_math.c
    src/c_numeric.c
    src/c_object.c
    src/c_range.c
    src/c_string.c
    src/class.c
    src/console.c
    src/error.c
    src/global.c
    src/hal.c
    src/keyvalue.c
    src/load.c
    src/mrblib.c
    src/rrt0.c
    src/symbol.c
    src/value.c
    src/vm.c
)
