cmake_minimum_required(VERSION 2.8.11)
project(ZRLogger)
include_directories("Abstract Graphics" "Abstract States" "Hearts Graphics" "Hearts States" "Utility")

add_executable (ZRLogger main.cpp "Abstract Graphics/OpenGLUtility.cpp" "Hearts States/HeartsDrawable.cpp" "Abstract Graphics/SOIL.c" "Hearts States/HeartsDriver.cpp" "Abstract Graphics/image_DXT.c" "Hearts States/HeartsEngine.cpp" "Abstract Graphics/image_helper.c" "Hearts States/HeartsMovable.cpp" "Abstract Graphics/stb_image_aug.c" "Hearts States/HeartsState.cpp" "Hearts Graphics/OpenGlPlayingCard.cpp" "Hearts Graphics/PlayingCard.cpp" "Utility/Random.cpp")

find_library(CORE_FOUNDATION CoreFoundation)
target_link_libraries(ZRLogger ${CORE_FOUNDATION})

find_library(GLEW glew)
target_link_libraries(ZRLogger ${GLEW})

find_library(GLFW3 glfw3)
target_link_libraries(ZRLogger ${GLFW3})

find_library(OPENGL OpenGL)
target_link_libraries(ZRLogger ${OPENGL})


SET(CMAKE_CXX_FLAGS "-std=c++11")
