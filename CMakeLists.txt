cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(photo-booth CXX)
find_package(OpenCV REQUIRED)
find_package(Cairo REQUIRED)
add_executable(photo-booth booths config.yaml sounds)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_LIBS opencv_highgui opencv_imgproc opencv_core)
include_directories(${CAIRO_INCLUDES})
target_link_libraries(photo-booth cairo ${OpenCV_LIBS})

install(TARGETS photo-booth
    RUNTIME DESTINATION bin)
install(FILES config.yaml
    DESTINATION /etc/photo-booth
    COMPONENT config)
install(FILES gnome-menu
    DESTINATION /usr/share/applications
    RENAME photo-booth.desktop)
install(DIRECTORY sounds
    DESTINATION share/photo-booth)
install(DIRECTORY build/locale
    DESTINATION share)
