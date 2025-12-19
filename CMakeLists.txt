cmake_minimum_required(VERSION 3.26)
project(proj01 VERSION 1.0)

cmake_policy(SET CMP0072 NEW)

# Look for custom Find*.cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Executable
add_executable(run01
    src/main.cpp
    src/glad.c
)

# Include directories
target_include_directories(run01 PRIVATE include)

# Link libraries
target_include_directories(run01 PRIVATE ${GLFW3_INCLUDE_DIR})
target_link_libraries(run01 PRIVATE ${GLFW3_LIBRARY} OpenGL::GL)

# Copy shaders to build directory
add_custom_target(CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_dependencies(run01 CopyShaders)
