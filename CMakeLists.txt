cmake_minimum_required(VERSION 3.25)

project(Bolt)
# set(CMAKE_BUILD_TYPE Debug)

include_directories(.)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(bolt bolt.cpp
                    LibBolt/Scaler.cpp
                    LibBolt/Symbol.cpp
                    LibBolt/List.cpp
                    LibBolt/Compiler.cpp
                    LibBolt/Instruction.cpp
                    LibBolt/Tokenizer.cpp
                    LibBolt/Block.cpp
                    LibBolt/Parser.cpp
                    LibBolt/Function.cpp
                    LibBolt/String.cpp
)
