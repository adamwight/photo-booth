cmake_minimum_required(VERSION 2.8)

project( booths )
find_package( OpenCV REQUIRED )
add_executable( booths booths )

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
#SET_TARGET_PROPERTIES(booths PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( booths ${OpenCV_LIBS} )
