# top level CMAKE sets flags, options and external libs location

cmake_minimum_required(VERSION 2.8)
project(engine)

# flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -flag") append to flags
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flag") append to flags
set(OpenGL_GL_PREFERENCE "LEGACY")

# options
option(BUILD_SHARED_LIBS "build the shared libs" on)
if(BUILD_SHARED_LIBS)
	option(USE_SHARED_GLFW "Use the shared glfw library" off)
	option(USE_SHARED_GLM "Use the shared glm library" off)
	option(USE_SHARED_ASSIMP "use the shadred assimp library" off)
	option(USE_SHARED_BULLET3 "use the shadred bullet3 library" off)
else()
	option(USE_SHARED_GLFW "Use the shared glfw library" on)
	option(USE_SHARED_GLM "Use the shared glm library" on)
	option(USE_SHARED_ASSIMP "use the shadred assimp library" on)
	option(USE_SHARED_BULLET3 "use the shadred bullet3 library" on)
endif()

# directories
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE PATH "")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(GLAD_DIR ${CMAKE_SOURCE_DIR}/externals/glad)
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/externals/glfw)
set(GLM_DIR ${CMAKE_SOURCE_DIR}/externals/glm)
set(STB_IMAGE_DIR ${CMAKE_SOURCE_DIR}/externals/stb_image)
set(ASSIMP_DIR ${CMAKE_SOURCE_DIR}/externals/assimp)
set(BULLET_DIR ${CMAKE_SOURCE_DIR}/externals/bullet3)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/externals/imgui)

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

#glad
set(GLAD_LIBRARIES gladstatic)
set(GLAD_INCLUDE_DIR ${GLAD_DIR}/include)
include_directories(SYSTEM ${GLAD_INCLUDE_DIR}) # SYSTEM to supress warnings

#glfw
if(USE_SHARED_GLFW)
	find_package(glfw3 3.2 REQUIRED)
else()
	set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)
endif()
set(GLFW_LIBRARIES glfw)
include_directories(SYSTEM ${GLFW_INCLUDE_DIR}) # SYSTEM to supress warnings

#glm
if(USE_SHARED_GLM)
	find_package(glm REQUIRED)
else()
	set(GLM_INCLUDE_DIR ${GLM_DIR})
endif()
include_directories(SYSTEM ${GLM_INCLUDE_DIR})

#stb_image
include_directories(SYSTEM ${STB_IMAGE_DIR})

#assimp
if(USE_SHARED_ASSIMP)
	find_package(assimp REQUIRED)
else()
	set(ASSIMP_INCLUDE_DIR ${ASSIMP_DIR}/include)
	set(ASSIMP_LIBRARIES assimp)
endif()
include_directories(SYSTEM ${ASSIMP_INCLUDE_DIR})

#bullet3
if(USE_SHARED_BULLET3)
	find_package(Bullet REQUIRED)
else()
	set(BULLET_INCLUDE_DIR ${BULLET_DIR}/src ${BULLET_DIR}/Extras/Serialize)
	set(BULLET_LIBRARIES BulletFileLoader BulletWorldImporter BulletDynamics BulletCollision LinearMath)
endif()
include_directories(SYSTEM ${BULLET_INCLUDE_DIR})
set(BULLET_LIBRARIES ${BULLET_LIBRARIES} BulletFileLoader BulletWorldImporter)

#imgui
set(IMGUI_LIBRARIES imgui-lib)
include_directories(SYSTEM ${IMGUI_DIR})

# external libraries
add_subdirectory(externals)

# src
add_subdirectory(src)
