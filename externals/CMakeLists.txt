# glad
add_subdirectory(glad)

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
	add_subdirectory(assimp)
endif()