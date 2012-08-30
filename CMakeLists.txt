cmake_minimum_required(VERSION 2.8)

project( booths CXX )
find_package( OpenCV REQUIRED )
add_executable( booths booths )

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

target_link_libraries( booths ${OpenCV_LIBS} )
