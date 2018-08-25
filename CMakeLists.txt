# top level CMAKE sets flags, options and external libs location

cmake_minimum_required(VERSION 2.8)
project(engine)

# flags
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -flag") append to flags
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flag") append to flags
set(OpenGL_GL_PREFERENCE "LEGACY")

# options
option(USE_SHARED_GLFW "Use the shared glfw library" on)
option(USE_SHARED_GLM "Use the shared glm library" on)
option(USE_SHARED_ASSIMP "use the shadred assimp library" on)

# directories
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(GLAD_DIR ${CMAKE_SOURCE_DIR}/externals/glad)
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/externals/glfw)
set(GLM_DIR ${CMAKE_SOURCE_DIR}/externals/glm)
set(STB_IMAGE_DIR ${CMAKE_SOURCE_DIR}/externals/stb_image)
set(ASSIMP_DIR ${CMAKE_SOURCE_DIR}/externals/assimp)

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

#glad
set(GLAD_LIBRARIES gladstatic)
set(GLAD_INCLUDE_DIRS ${GLAD_DIR}/include)
include_directories(${GLAD_INCLUDE_DIRS})

#glfw
if(USE_SHARED_GLFW)
	find_package(glfw3 3.2 REQUIRED)
else()
	set(GLFW_INCLUDE_DIRS ${GLFW_DIR}/include)
endif()
set(GLFW_LIBRARIES glfw)
include_directories(${GLFW_INCLUDE_DIRS})

#glm
if(USE_SHARED_GLM)
	find_package(glm REQUIRED)
else()
	set(GLM_INCLUDE_DIRS ${GLM_DIR}/include)
endif()
include_directories(${GLM_INCLUDE_DIRS})

#stb_image
include_directories(${STB_IMAGE_DIR})

#assimp
if(USE_SHARED_ASSIMP)
	find_package(assimp REQUIRED)
else()
	set(ASSIMP_INCLUDE_DIRS ${ASSIMP_DIR}/include)
	set(ASSIMP_LIBRARIES "assimp")
endif()
include_directories(${ASSIMP_INCLUDE_DIRS})

# external libraries
add_subdirectory(externals)

# src
add_subdirectory(src)