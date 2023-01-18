cmake_minimum_required(VERSION 3.23)
project(boids)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS graphics audio network system window REQUIRED)
add_executable(${PROJECT_NAME} src/main.cpp src/boid.h)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio sfml-network sfml-system sfml-window)
