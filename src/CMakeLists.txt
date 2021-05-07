cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracing main.cpp Camera.h Object.h Vector.h Sphere.h global.h Triangle.h Scene.cpp
        Scene.h BVH.cpp BVH.h Bounds3.h Ray.h Material.h Intersection.h stb_image_write.h
        Renderer.cpp Renderer.h)
