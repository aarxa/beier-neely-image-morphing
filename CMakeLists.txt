cmake_minimum_required(VERSION 3.16)
project(GraphicsAssignments)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Adjust the path to your additional CMake modules, if any
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include Eigen
if(WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/eigen)
else()
    find_package(Eigen3 REQUIRED)
    include_directories(${EIGEN3_INCLUDE_DIR})
endif()

# Include libigl
# Assuming libigl is a subdirectory in your project
add_subdirectory(libigl)
igl_include(core)
igl_include(glfw)
igl_include(opengl)
igl_include(stb)
igl_include(imgui)


# add all headers
include_directories("include/")

# Create a new executable for ImageMorfingMethod
file(GLOB SRCFILES source/*.cpp)
add_library(core ${SRCFILES})

add_executable(ImageMorfingMethod "main.cpp")
target_link_libraries(ImageMorfingMethod PUBLIC core igl::core igl::glfw igl::opengl igl::stb PRIVATE igl::imgui)