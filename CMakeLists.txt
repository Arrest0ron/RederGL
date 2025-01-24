cmake_minimum_required(VERSION 3.11)
project(RederGL)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use GLVND for OpenGL
set(OpenGL_GL_PREFERENCE GLVND)

# Find OpenGL
message("Searching for OpenGL (GLVND)...")
find_package(OpenGL REQUIRED)
message("OpenGL found.")

message("Fetching GLFW...")
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)
message("GLFW fetched!")


message("Adding GLAD...")
set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/glad/include)
set(GLAD_SOURCE ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)

# Add executable
message("Adding executables...")
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/RederGL.cpp ${GLAD_SOURCE})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_INCLUDE_DIR})

# Link libraries
message("Target linking libraries...")
target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw)