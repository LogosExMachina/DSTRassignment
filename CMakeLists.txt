cmake_minimum_required(VERSION 3.16)
project(DSTRassignment CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(
  include
  include/shared
  include/dynamicArray
  include/linkedList
)

add_executable(solution1 src/dynamicArray/solution1.cpp src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/date.cpp src/shared/strUtils.cpp)
add_executable(solution2 src/dynamicArray/solution2.cpp src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/date.cpp src/shared/strUtils.cpp)
add_executable(solution3 src/dynamicArray/solution3.cpp src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/date.cpp src/shared/strUtils.cpp)
add_executable(linkedListMain src/linkedList/solution.cpp src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/strUtils.cpp)
