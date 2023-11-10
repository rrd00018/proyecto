cmake_minimum_required(VERSION 3.26)
project(proyecto)

set(CMAKE_CXX_STANDARD 17)
# STAGE 1: CREATING THE EXECUTABLE


# Copy shaders to build directory
file(GLOB SHADER_SOURCES "shaders/*.glsl")
foreach(SHADER ${SHADER_SOURCES})
    file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

# Set OpenGL_GL_PREFERENCE explicitly
set(OpenGL_GL_PREFERENCE "GLVND")

# Locating libraries
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(GLEW)
FIND_PACKAGE(glfw3)

# OpenGL is required
IF (OPENGL_FOUND)
    message(STATUS "OpenGL sources found")
ELSE()
    message(FATAL_ERROR "OpenGL not found")
ENDIF ()

# If GLEW is not found, add sources from library files
IF (GLEW_FOUND)
    message(STATUS "GLEW sources found")
ELSE()
    message(STATUS "GLEW not found. Adding sources from lib/glew-2.1.0/build/cmake...")
    add_subdirectory(${PROJECT_SOURCE_DIR}/include/glew/build/cmake)
ENDIF ()

# If GLFW is not found, add sources from library files
IF (glfw3_FOUND)
    message(STATUS "GLFW3 sources found")
ELSE()
    message(STATUS "GLFW3 not found. Adding sources from lib/glfw-3.3.8...")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${PROJECT_SOURCE_DIR}/include/glfw3)
ENDIF ()

# Create executable
add_executable(main.cpp 3dstudio.h geometryrender.cpp geometryrender.h openglwindow.cpp openglwindow.h)

# STAGE 2: LINKING LIBRARIES
# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -ggdb3 -fno-inline -Wall -std=c++11")
    message(STATUS ${OPENGL_INCLUDE_DIR})
# Link OpenGL
IF (OPENGL_FOUND)
    message(STATUS "Linking OpenGL from system (" ${OPENGL_INCLUDE_DIR} ") (" ${OPENGL_LIBRARIES} ")")
    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(${TARGET} OpenGL::GL)
ELSE ()
    message(FATAL_ERROR "OpenGL not found")
ENDIF ()

# Link GLEW
IF (GLEW_FOUND)
    message(STATUS "Linking GLEW from system (" ${GLEW_INCLUDE_DIRS} ") (" ${GLEW_LIBRARIES} ")")
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(${TARGET} ${GLEW_LIBRARIES})
ELSE ()
    message(STATUS "Linking GLEW from lib/glew-2.1.0/build/cmake...")
    include_directories(${TARGET} ${PROJECT_SOURCE_DIR}/lib/glew/include)
    target_link_libraries(${TARGET} glew_s)
ENDIF ()

# Link GLFW
IF (glfw3_FOUND)
    message(STATUS "Linking GLFW from system")
    target_link_libraries(${TARGET} glfw)
ELSE ()
    message(STATUS "Linking GLFW from lib/glfw-3.3.8...")
    target_link_libraries(${TARGET} glfw)
ENDIF ()