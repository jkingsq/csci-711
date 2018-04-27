cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE debug)
project(Raytracer)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
add_library(Vector Vector.c)
add_library(Ray Ray.c)
add_library(Matrix Matrix.c)
add_library(Figure Figure.c)
add_library(Solid Solid.c)
add_library(Reflect Reflect.c)
add_library(Shader Shader.c)
add_executable(Raytracer main.c)

target_link_libraries(Ray m)

target_link_libraries(Raytracer ${SDL2_LIBRARIES})
target_link_libraries(Raytracer m)

target_link_libraries(Reflect Solid)

target_link_libraries(Shader Ray)

target_link_libraries(Raytracer Vector)
target_link_libraries(Raytracer Matrix)
target_link_libraries(Raytracer Figure)
target_link_libraries(Raytracer Solid)
target_link_libraries(Raytracer Reflect)
target_link_libraries(Raytracer Shader)
