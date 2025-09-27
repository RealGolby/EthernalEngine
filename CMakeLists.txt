cmake_minimum_required(VERSION 3.16)
project(EthernalEngine)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# Add executable
add_executable(EthernalEngine src/main.cpp src/glad.c
        src/Rendering/Shader.cpp
        src/Rendering/Shader.h
        include/stb/stb_image.h
        src/Rendering/Images/Texture.cpp
        src/Rendering/Images/Texture.h
)

# Link libraries
target_link_libraries(EthernalEngine PRIVATE glfw opengl32)
include_directories(include)
