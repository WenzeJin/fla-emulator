cmake_minimum_required(VERSION 3.15.0)
project(fla)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(${PROJECT_SOURCE_DIR}/fla-project/include)

file(GLOB_RECURSE CPP_SRC ${PROJECT_SOURCE_DIR}/fla-project/src/*.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(fla ${CPP_SRC})

