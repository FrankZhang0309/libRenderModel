cmake_minimum_required(VERSION 3.10)

project(RenderModel)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenCV REQUIRED)

SET(CMAKE_C_COMPILER g++)
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

set( CMAKE_BUILD_TYPE "Debug" )

include_directories(./)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library( model_renderer ModelRenderer.cpp ModelRenderer.h Model.cpp Model.h Shader.h)
add_library( model_renderer_shared SHARED ModelRenderer.cpp ModelRenderer.h Model.cpp Model.h Shader.h)

target_link_libraries(model_renderer glfw ${GLEW_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(model_renderer_shared glfw ${GLEW_LIBRARIES} ${OpenCV_LIBS})

add_executable(example main.cpp)

target_link_libraries(example model_renderer -lGL)