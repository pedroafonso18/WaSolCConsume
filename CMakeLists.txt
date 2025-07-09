cmake_minimum_required(VERSION 3.31)
project(WaSolCConsume C)

set(CMAKE_C_STANDARD 23)

add_executable(WaSolCConsume main.c
        config.c
        config.h
        include/base.h)
