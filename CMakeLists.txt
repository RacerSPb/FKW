cmake_minimum_required(VERSION 3.28)

project(search_engine VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(json-develop)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()