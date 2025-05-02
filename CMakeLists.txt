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

add_executable(solution_array_Santiago_1 src/dynamicArray/solution_Santiago_1.cpp src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/date.cpp src/shared/strUtils.cpp)
add_executable(solution_array_Santiago_2 src/dynamicArray/solution_Santiago_2.cpp src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/date.cpp src/shared/strUtils.cpp)
add_executable(solution_array_Santiago_3 src/dynamicArray/solution_Santiago_3.cpp src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/date.cpp src/shared/strUtils.cpp)
add_executable(solution_array_Jaeden src/dynamicArray/solution_Jaeden src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/date.cpp src/shared/strUtils.cpp)
add_executable(solution_linkedlist_Wenyi src/linkedList/solution_Wenyi.cpp src/shared/csvParser.cpp src/shared/dataTable.cpp src/shared/strUtils.cpp)
