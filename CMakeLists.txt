cmake_minimum_required(VERSION 3.22)
project(simplculator C)

set(CMAKE_C_STANDARD 11)
if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

file(GLOB_RECURSE SOURCES "src/**.c")

include_directories("include")
add_executable("simplculator" ${SOURCES})
