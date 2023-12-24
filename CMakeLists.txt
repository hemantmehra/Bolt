cmake_minimum_required(VERSION 3.25)

project(Bolt)

include_directories(.)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(bolt bolt.cpp
                    LibBolt/Scaler.cpp
                    LibBolt/Symbol.cpp
                    LibBolt/Expression.cpp
                    LibBolt/Compiler.cpp
)
