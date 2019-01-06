# glad
add_subdirectory(glad)

# imgui
add_subdirectory(imgui)

# glfw
if(NOT USE_SHARED_GLFW)
# disable glfw test, docs and examples
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory(glfw)
endif()

#glm
if(NOT USE_SHARED_GLM)
	# set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
	# add_subdirectory(glm) header only
endif()

#assimp
if(NOT USE_SHARED_ASSIMP)
	set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
	set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	# set(INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE) off for release build
	add_subdirectory(assimp)
endif()

#bullet3
if(NOT USE_SHARED_BULLET3)
	set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "" FORCE)
	set(USE_GLUT OFF CACHE BOOL "" FORCE)
	set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
	set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
	set(BUILD_ENET OFF CACHE BOOL "" FORCE)
	set(BUILD_CLSOCKET OFF CACHE BOOL "" FORCE)
	set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
	set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
	set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE) # don't want/need all the extras
	set(INSTALL_LIBS OFF CACHE BOOL "" FORCE)
	set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
	set(INSTALL_CMAKE_FILES OFF CACHE BOOL "" FORCE)

	# version required for bullet3/extras
	set(BULLET_VERSION "")
	add_subdirectory(bullet3)
	# extracting bullet version from bullet3 directory for bullet3/Extras
	get_directory_property(BULLET_VERSION DIRECTORY bullet3 DEFINITION BULLET_VERSION)
	add_subdirectory(bullet3/Extras/Serialize/BulletFileLoader)
	add_subdirectory(bullet3/Extras/Serialize/BulletWorldImporter)
endif()
