cmake_minimum_required(VERSION 2.8)
project(test)

find_package(OpenCV REQUIRED)

if(NOT WIN32)
ADD_DEFINITIONS("-std=c++0x -O3")
endif(NOT WIN32)

include_directories(src)
FILE(GLOB_RECURSE sourcefiles "src/*.cpp")
add_executable( KCF runtracker.cpp ${sourcefiles})
target_link_libraries( KCF ${OpenCV_LIBS})

add_executable(test test.cpp ${sourcefiles})
target_link_libraries(test ${OpenCV_LIBS})




