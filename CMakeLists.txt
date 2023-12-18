cmake_minimum_required(VERSION 2.8)

project(Converter)
# set(CMAKE_DEBUG_POSTFIX d)

add_executable(Converter
  ./source/main.cpp
  ./source/Bigint.h
  ./source/Bigint.cpp
)


# set_target_properties(Converter PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
