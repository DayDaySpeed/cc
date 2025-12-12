cmake_minimum_required(VERSION 3.10)   # 要求的最低 CMake 版本
project(CC)                   # 项目名称和语言

set(CMAKE_C_STANDARD 11)               # 指定 C 标准

# 定义可执行文件，包含多个源文件
add_executable(syntax
    main.c
    lexical.c
    syntax.c
)
