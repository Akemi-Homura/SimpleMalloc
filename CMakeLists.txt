cmake_minimum_required(VERSION 3.9)
project(SimpleMalloc C)

set(CMAKE_C_STANDARD 11)

add_executable(SimpleMalloc main.c simple_malloc.c simple_free.c)
