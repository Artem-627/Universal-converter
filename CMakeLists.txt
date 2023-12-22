cmake_minimum_required(VERSION 3.20)

project(Converter VERSION 1.0.0 LANGUAGES CXX)
# set(CMAKE_DEBUG_POSTFIX d)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(Converter
  ./source/main.cpp
  ./source/Bigint.h
  ./source/Bigint.cpp
  ./source/BigFraction.h
  ./source/BigFraction.cpp
  ./source/BigNumber.h
  ./source/BigNumber.cpp
  ./source/App.h
  ./source/App.cpp
)


target_link_libraries(Converter PRIVATE Qt6::Widgets)
# set_target_properties(Converter PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})make_minimum_required(VERSION 3.20)

# project(Converter VERSION 1.0.0 LANGUAGES CXX)
# set(CMAKE_DEBUG_POSTFIX d)

# add_executable(Converter
#   ./source/main.cpp
#   ./source/Bigint.h
#   ./source/Bigint.cpp
#   ./source/BigFraction.h
#   ./source/BigFraction.cpp
#   ./source/BigNumber.h
#   ./source/BigNumber.cpp
# )


# set_target_properties(Converter PROPERTIES DEBUG_POSTFIX ${CM
