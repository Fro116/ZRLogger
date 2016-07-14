cmake_minimum_required(VERSION 2.8.11)
project(ZRLogger)
include_directories("Abstract Graphics" "Abstract States" "States" "Utility")

add_executable (ZRLogger main.cpp "Abstract Graphics/OpenGLUtility.cpp" "Abstract States/Drawable.cpp" "Abstract Graphics/SOIL.c" "Abstract Graphics/image_DXT.c" "Abstract States/GameEngine.cpp" "Abstract States/GameDriver.cpp" "Abstract Graphics/image_helper.c" "Abstract States/Movable.cpp" "Abstract Graphics/stb_image_aug.c" "States/OverworldState.cpp" "States/OverworldSelector.cpp" "Abstract Graphics/OpenGLDoubleSidedRectangle.cpp" "Abstract Graphics/OpenGLRectangle.cpp")

find_library(CORE_FOUNDATION CoreFoundation)
target_link_libraries(ZRLogger ${CORE_FOUNDATION})

find_library(GLEW glew)
target_link_libraries(ZRLogger ${GLEW})

find_library(GLFW3 glfw3)
target_link_libraries(ZRLogger ${GLFW3})

find_library(OPENGL OpenGL)
target_link_libraries(ZRLogger ${OPENGL})


SET(CMAKE_CXX_FLAGS "-std=c++11")
