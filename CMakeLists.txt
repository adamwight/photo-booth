cmake_minimum_required(VERSION 2.8)

project(photo-booth CXX)
find_package(OpenCV REQUIRED)
find_package(Qt4 REQUIRED QtCore)
add_executable(photo-booth booths config.yaml sounds)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_LIBS opencv_highgui opencv_imgproc opencv_core)
include_directories(${QT4_INCLUDES})
target_link_libraries(photo-booth Qt4::QtCore ${OpenCV_LIBS})

install(TARGETS photo-booth
    RUNTIME DESTINATION bin)
install(FILES config.yaml
    DESTINATION /etc/photo-booth
    COMPONENT config)
install(DIRECTORY sounds
    DESTINATION share/photo-booth)
install(DIRECTORY build/locale
    DESTINATION share)
