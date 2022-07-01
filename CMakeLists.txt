cmake_minimum_required(VERSION 3.22)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracer main.cpp Dependencies/stb_image.h Dependencies/stb_image_write.h Core/Math.h Core/RayTracer.cpp Core/RayTracer.h Core/Color.h Core/Ray.cpp Core/Ray.h Core/Camera.cpp Core/Camera.h Core/Scene.cpp Core/Scene.h Core/Object.cpp Core/Object.h Core/Sphere.h Core/Material.cpp Core/Material.h)
