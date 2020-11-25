#linux
#cmake -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ -Bbuild -H.  -DCMAKE_EXPORT_COMPILE_COMMANDS=1

#windows
#cmake -H. -G "Visual Studio 16 2019" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -Bbuild

cmake_minimum_required(VERSION 3.18)
project(brocolio LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-mavx2")

add_library(stopwatch
  stopwatch.cpp)
add_executable(test
  test.cpp)
