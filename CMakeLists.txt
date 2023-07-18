cmake_minimum_required(VERSION 3.22)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracer main.cpp Dependencies/stb_image.h Dependencies/stb_image_write.h Core/Math.h Core/RayTracer.cpp Core/RayTracer.h Core/Color.h Core/Ray.h Core/Camera.cpp Core/Camera.h Core/Scene.h Core/Object.h Core/Sphere.h Core/Material.h Core/Light.h Core/TriangleMesh.h Core/Bound.h Core/BVH.cpp Core/BVH.h Core/Intersection.h Core/Material.cpp Core/Bound.cpp)
