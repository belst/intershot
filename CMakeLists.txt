cmake_minimum_required(VERSION 2.8.11)

project(intershot)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

file(GLOB SRC_FILES
    src/*.cpp
    src/*.h)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
