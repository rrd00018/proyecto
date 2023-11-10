cmake_minimum_required(VERSION 3.21)
project(proyecto)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
# Copy shaders to build directory
file(GLOB SHADER_SOURCES "shaders/*.glsl")
foreach(SHADER ${SHADER_SOURCES})
    file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

# Set OpenGL_GL_PREFERENCE explicitly
set(OpenGL_GL_PREFERENCE "GLVND")
set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/glfw3")

# Locating libraries
find_package(OpenGL)
find_package(GLEW)
find_package(glfw3)

# Create executable
add_executable(proyecto
        main.cpp
        3dstudio.h
        geometryrender.cpp
        geometryrender.h
        openglwindow.cpp
        openglwindow.h
)

# STAGE 2: LINKING LIBRARIES
# Set compiler flags

IF (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_directories(proyecto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(proyecto OpenGL::GL GLEW::GLEW glfw)
ENDIF ()

IF (CMAKE_SYSTEM_NAME STREQUAL "Windows")
# Link OpenGL
IF (OPENGL_FOUND)
    message(STATUS "Linking OpenGL from system (" ${OPENGL_INCLUDE_DIR} ") (" ${OPENGL_LIBRARIES} ")")
    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(proyecto OpenGL::GL)
ELSE ()
    message(FATAL_ERROR "OpenGL not found")
ENDIF ()

# Link GLEW
# Link GLEW
IF (GLEW_FOUND)
    message(STATUS "Linking GLEW from system (" ${GLEW_INCLUDE_DIRS} ") (" ${GLEW_LIBRARIES} ")")
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(proyecto ${GLEW_LIBRARIES})
ELSE ()
    message(STATUS "Linking GLEW from lib/glew-2.1.0/build/cmake...")
    target_include_directories(proyecto PRIVATE ${PROJECT_SOURCE_DIR}/include/glew/include)
    target_link_libraries(proyecto ${PROJECT_SOURCE_DIR}/include/glew/lib/glew32.lib)
ENDIF ()


# Link GLFW
IF (glfw3_FOUND)
    message(STATUS "Linking GLFW from system")
    target_link_libraries(proyecto glfw)
ELSE ()
    message(STATUS "Linking GLFW from lib/glfw-3.3.8...")
    target_include_directories(proyecto PRIVATE ${PROJECT_SOURCE_DIR}/include/glfw3/include)
    target_link_libraries(proyecto ${PROJECT_SOURCE_DIR}/include/glfw3/lib/glfw3dll.lib)
ENDIF ()
ENDIF ()