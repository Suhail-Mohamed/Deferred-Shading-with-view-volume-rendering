cmake_minimum_required(VERSION 2.6)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Set the target plaform of the project to be win32
if(WIN32)
	set(CMAKE_GENERATOR_PLATFORM win32)
endif(WIN32)

# Name of project
set(PROJ_NAME final) 
project(${PROJ_NAME})

#Include both the src (.cpp) and include (.h) directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Specify project source files
set(SRCS
    ${PROJECT_SOURCE_DIR}/main.cpp

    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/cylinder.cpp
    ${PROJECT_SOURCE_DIR}/src/graphicsObject.cpp
    ${PROJECT_SOURCE_DIR}/src/Light.cpp
    ${PROJECT_SOURCE_DIR}/src/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/Solution.cpp
    ${PROJECT_SOURCE_DIR}/src/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/surface.cpp


    ${PROJECT_SOURCE_DIR}/include/camera.h
    ${PROJECT_SOURCE_DIR}/include/cylinder.h
    ${PROJECT_SOURCE_DIR}/include/graphicsObject.h
    ${PROJECT_SOURCE_DIR}/include/Light.h
    ${PROJECT_SOURCE_DIR}/include/Shader.h
    ${PROJECT_SOURCE_DIR}/include/Solution.h
    ${PROJECT_SOURCE_DIR}/include/sphere.h
    ${PROJECT_SOURCE_DIR}/include/surface.h
)

# Add executable based on the source files
add_compile_options(-Wall -Wextra -Wpedantic )
add_executable(${PROJ_NAME} ${SRCS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

#Include the header files
include_directories(${LIBRARY_PATH}/include)

#find libraries for linux/Mac OS users
if(NOT WIN32)
    find_library(GLUT_LIBRARY glut)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLU_LIBRARY  GLU)

    message(" OPENGL => " ${OPENGL_INCLUDE_DIR})
    message("   GLUT => " ${GLUT_LIBRARY})
    message("   GLEW => " ${GLEW_LIBRARY})
    message("   GLU  => " ${GLU_LIBRARY})
#Find libraries for windows users
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLUT_LIBRARY freeglut HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)

    file(COPY ${LIBRARY_PATH}/lib/freeglut.dll
         DESTINATION ${CMAKE_BINARY_DIR})
endif(NOT WIN32)

target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLUT_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLU_LIBRARY})

