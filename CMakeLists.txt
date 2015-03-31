cmake_minimum_required(VERSION 2.8)

project(booths CXX)
find_package(OpenCV REQUIRED)
add_executable(booths booths config.yaml sounds)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_LIBS opencv_highgui opencv_imgproc opencv_core)
target_link_libraries(booths ${OpenCV_LIBS})

install(TARGETS booths
    RUNTIME DESTINATION bin)
install(FILES config.yaml
    DESTINATION /etc/booths
    COMPONENT config)
install(DIRECTORY sounds
    DESTINATION share/booths)
