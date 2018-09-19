# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/renderEngine)

# NOTE: Don't forget to update this after new .cpp file
# renderEngine
add_library(engine-lib
	renderEngine/display_manager.cpp
	renderEngine/entity_renderer.cpp
	renderEngine/terrain_renderer.cpp
	renderEngine/render_engine.cpp

	entities/entity.cpp
	entities/camera.cpp

	model/model.cpp
	model/mesh.cpp
	model/textured_mesh.cpp
	model/texture.cpp

	shader/shader.cpp
	shader/buffer/uniform_buffer.cpp
	shader/entity_shader.cpp
	shader/terrain_shader.cpp
	shader/debug_physics_shader.cpp

	terrain/terrain.cpp
	terrain/terrain_grid.cpp

	utils/image.cpp
	utils/mtl_parser.cpp

	physics/collision_shape.cpp
	physics/physics.cpp
	physics/physics_body.cpp
	physics/rigid_body.cpp
	physics/debug_drawer.cpp
	physics/physics_importer.cpp
)

target_link_libraries(
	engine-lib
	${OPENGL_LIBRARIES}
	${GLAD_LIBRARIES}
	${GLFW_LIBRARIES}
	${ASSIMP_LIBRARIES}
	${BULLET_LIBRARIES}
	BulletWorldImporter
	BulletFileLoader
	dl
)

# add to engine-lib
# if(ENABLE_SCRIPTING)
# target_link_libraries(engine-lib script-lib)
