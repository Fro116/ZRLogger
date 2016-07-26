cmake_minimum_required(VERSION 2.8.11)
project(ZRLogger)
include_directories("Abstract Graphics" "Abstract States" "States" "Utility" "Object Recognition")

add_executable (ZRLogger main.cpp "Abstract Graphics/OpenGLUtility.cpp" "Abstract States/Drawable.cpp" "Abstract Graphics/SOIL.c" "Abstract Graphics/image_DXT.c" "Abstract States/GameEngine.cpp" "Abstract States/GameDriver.cpp" "Abstract Graphics/image_helper.c" "Abstract States/Movable.cpp" "Abstract Graphics/stb_image_aug.c" "States/OverworldState.cpp" "States/DungeonState.cpp" "States/DungeonMarker.cpp" "States/TriforceMarker.cpp" "States/ItemMarker.cpp" "States/BonusCaveMarker.cpp" "States/HeartMarker.cpp" "States/DungeonDoor.cpp" "States/ZeldaFindingScreen.cpp" "States/OverworldMarker.cpp" "States/DungeonSelector.cpp" "States/OverworldSelector.cpp" "States/ZRDriver.cpp" "Abstract Graphics/OpenGLRectangle.cpp" "Object Recognition/ZeldaImageProcessor.cpp" "Object Recognition/ImageHandler.cpp" "Object Recognition/ZeldaInformationHandler.cpp")

find_library(CORE_FOUNDATION CoreFoundation)
target_link_libraries(ZRLogger ${CORE_FOUNDATION})

find_library(GLEW glew)
target_link_libraries(ZRLogger ${GLEW})

find_library(GLFW3 glfw3)
target_link_libraries(ZRLogger ${GLFW3})

find_library(OPENGL OpenGL)
target_link_libraries(ZRLogger ${OPENGL})

find_library(APP_SER ApplicationServices)
target_link_libraries(ZRLogger ${APP_SER})

SET(CMAKE_CXX_FLAGS "-std=c++11")
