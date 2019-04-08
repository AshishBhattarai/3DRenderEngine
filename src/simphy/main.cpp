#include <iostream>
#include <cstdlib>

#include "renderEngine/display_manager.hpp"
#include "renderEngine/render_engine.hpp"
#include "entities/camera.hpp"
#include "model/model.hpp"
#include "entities/entity.hpp"
#include "entities/spot_light.hpp"
#include "entities/lamp.hpp"
#include "utils/slogger.hpp"
#include "utils/image.hpp"
#include "terrain/terrain_grid.hpp"

#include "physics/physics_world.hpp"
#include "physics/collision_shape/collision_shape.hpp"
#include "entities/physics_entity.hpp"
#include "physics/physics_body/static_body.hpp"
#include "physics/physics_body/rigid_body.hpp"
#include "physics/debug_drawer.hpp"
#include "physics/physics_importer.hpp"

#include "gui/gui.hpp"
#include "model/skybox.hpp"

#include "framebuffer/framebuffer.hpp"
#include "framebuffer/post_processor.hpp"

// #include "framebuffer/framebuffer.hpp"

void processInput(DisplayManager& display, Camera& camera) {
  // move camera
  if(display.getKey(GLFW_KEY_W) == GLFW_PRESS) {
    camera.processKeyboard(FORWARD, display.getDelta());
  }
  if(display.getKey(GLFW_KEY_S) == GLFW_PRESS) {
		camera.processKeyboard(BACKWARD, display.getDelta());
	}
	if(display.getKey(GLFW_KEY_A) == GLFW_PRESS) {
		camera.processKeyboard(LEFT, display.getDelta());
	}
	if(display.getKey(GLFW_KEY_D) == GLFW_PRESS) {
		camera.processKeyboard(RIGHT, display.getDelta());
  }
}

// check possible opengl errors
void errorCheck() {
	int ogl_err = glGetError();
	if(ogl_err)
		CSLOG("Opengl Error: ", ogl_err);
}

int main() {
	// create display
	DisplayManager& display = DisplayManager::getInstance();
	if(display.isFailed()) return EXIT_FAILURE;
	display.setCursorMode(GLFW_CURSOR_DISABLED);
	bool cursor_disabled = true;
	float pause = false;

	display.addKeyCallback([&cursor_disabled, &pause, &display](int key, int action, int) {
		if(action == GLFW_PRESS) {
			if(key == GLFW_KEY_LEFT_ALT) {
				cursor_disabled = !cursor_disabled;
				if(cursor_disabled) {
					display.setCursorMode(GLFW_CURSOR_DISABLED);
				} else {
					display.setCursorMode(GLFW_CURSOR_NORMAL);
				}
			}
			if(key == GLFW_KEY_SPACE) {
				pause = !pause;
			}
		}
	});

	// camera
	Camera camera(glm::vec3(400.0f, 2.0f, 200.0f));
	display.addMouseMovementCallback([&camera, &cursor_disabled, &display] (float xoffset, float yoffset) {
		if(cursor_disabled)
			camera.processMouseMovement(xoffset, yoffset, display.getDelta());
	});

	// loadmodel
	std::shared_ptr<Model> cube = std::make_shared<Model>("models/Colored/cube.obj", Model::MATERIAL_COLOR);
	std::shared_ptr<Model> football = std::make_shared<Model>("models/Colored/football.obj", Model::MATERIAL_COLOR);
	std::shared_ptr<Model> steelball = std::make_shared<Model>("models/Colored/steel.obj", Model::MATERIAL_COLOR);
	std::shared_ptr<Model> lamp = std::make_shared<Model>("models/streetlamp/streetlamp.obj", Model::MATERIAL_TEXTURE);
	std::shared_ptr<Model> table = std::make_shared<Model>("models/table/console_table.obj", Model::MATERIAL_TEXTURE);

	// light
	Light sun;
	sun.setPosition(glm::vec3(500000.0f, 500000.0f, 10000.0f));
	sun.setDiffuse(glm::vec3(0.0f));
	sun.setSpecular(glm::vec3(0.0f));

	// terrains
	TerrainGrid terrainGrid(1, 1, 800, 256);
	{
		Texture::Map texMap;
		texMap[Texture::BLEND_MAP] = std::make_shared<Texture>("textures/blendMap.png", Texture::BLEND_MAP, true);
		texMap[Texture::DIFFUSE_RED_MAP] = std::make_shared<Texture>("textures/mud.png", Texture::DIFFUSE_RED_MAP, true);
		texMap[Texture::DIFFUSE_GREEN_MAP] = std::make_shared<Texture>("textures/green.jpg", Texture::DIFFUSE_GREEN_MAP, true);
		texMap[Texture::DIFFUSE_BLUE_MAP] = std::make_shared<Texture>("textures/path.png", Texture::DIFFUSE_BLUE_MAP, true);
		texMap[Texture::DIFFUSE_BLACK_MAP] = std::make_shared<Texture>("textures/grass.png", Texture::DIFFUSE_BLACK_MAP, true);

		Image heightMap("heightMaps/ss.png", false);

		terrainGrid.addTerrain(0, 0, heightMap, texMap);
		terrainGrid.getTerrain(0, 0)->getPhysicsBody()->addToWorld();
	}

	// physics world
	PhysicsWorld& world = PhysicsWorld::getInstance();

	// collision Shape
	std::shared_ptr<CollisionShape> boxShape = std::make_shared<CollisionShape>(glm::vec3(1.0f));
	std::shared_ptr<CollisionShape> sphereShape = std::make_shared<CollisionShape>(0.9f);
	std::shared_ptr<CollisionShape> cubeShape = std::make_shared<CollisionShape>(glm::vec3(0.2f, 0.2f, 0.4f));

	// 1000 entities
	std::vector<std::unique_ptr<Entity>> entities;
	Entity* debug_entity = nullptr;
	entities.reserve(500);
	srand(time(0));
	entities.emplace_back(std::make_unique<Entity>(table, glm::vec3(camera.getPosition().x+40.0f, 0.0f, camera.getPosition().z-2.0f)));
	entities.back()->setScale(0.12f);
	entities.back()->updateAABB();
	entities.emplace_back(std::make_unique<Entity>(table, glm::vec3(camera.getPosition().x+40.0f, 0.0f, camera.getPosition().z+2.0f)));
	entities.back()->setScale(0.12f);
	entities.back()->updateAABB();
	auto drawBlocks = [&entities, &cube, &cubeShape](const glm::vec3& pos, float mass = 2.0f) {
		float x = 0.0f, y = 0.0f, z = 0.0f;
		for(int i = 0; i < 8; ++i) {
			for(int j = 0; j < 8; ++j) {
				for(int k = 0; k < 8; ++k) {
					x =  0.5f*i; y = 0.4f*j; z = 0.9*k;

					std::unique_ptr<PhysicsEntity> phy_entity = std::make_unique<PhysicsEntity>(cube, cubeShape, glm::vec3(pos.x+x, y+0.2f, pos.z+z), glm::vec3(0.0f), mass);
					PhysicsBody* physicsBody = phy_entity->getPhysicsBody();
					physicsBody->setFriction(1.0f);
					entities.emplace_back(std::move(phy_entity));
				}
			}
		}
	};

	glm::vec3 block_pos = camera.getPosition() + glm::vec3(0.0f, 0.0f, 5.0f);
	drawBlocks(block_pos);

	float force = 0.0f;
	float friction = 0.0f;
	float mass = 1.0f;
	float brick_mass = 2.0f;
	bool steel = false;
	bool space = false;

	display.addMouseButtonCallback([&cursor_disabled, &steelball, &steel, &football, &camera, &sphereShape, &force, &friction, &mass, &entities](int key, int action, int) {
		if(action == GLFW_PRESS) {
			if(key == GLFW_MOUSE_BUTTON_1 && cursor_disabled) {
				std::unique_ptr<PhysicsEntity> phy_entity;
				float m = (mass > 0.0f)? mass : 1.0f;
				if(steel) {
					phy_entity = std::make_unique<PhysicsEntity>(
						steelball, sphereShape, camera.getPosition(), glm::vec3(0.0f), m);
				} else {
					phy_entity = std::make_unique<PhysicsEntity>(
						football, sphereShape, camera.getPosition(), glm::vec3(0.0f), m
					);
				}

				RigidBody* rigidBody = static_cast<RigidBody*>(phy_entity->getPhysicsBody());
				phy_entity->getPhysicsBody()->setRollingFriction(friction);
				// rigidBody->setRestitution(0.8f);
				glm::vec3 launch = -camera.getFrontDirection() * force;
				rigidBody->applyCentralImpulse(launch);
				entities.emplace_back(std::move(phy_entity));
			}
		}
	});

	// skyboxes
	// SkyboxPath sb_path;
	SkyboxPath sb1_path;
	sb1_path.left = "textures/sky1/left.png";
	sb1_path.right = "textures/sky1/right.png";
	sb1_path.top = "textures/sky1/up.png";
	sb1_path.bottom = "textures/sky1/down.png";
	sb1_path.front = "textures/sky1/front.png";
	sb1_path.back = "textures/sky1/back.png";
	Skybox sbox(sb1_path, 2.0f);

	SkyboxPath sb2_path;
	sb2_path.left = "textures/sky2/left.png";
	sb2_path.right = "textures/sky2/right.png";
	sb2_path.top = "textures/sky2/up.png";
	sb2_path.bottom = "textures/sky2/down.png";
	sb2_path.front = "textures/sky2/front.png";
	sb2_path.back = "textures/sky2/back.png";
	Skybox ssbox(sb2_path, 2.0f);

	// renderEngine
	RenderEngine renderEngine(&camera, &sun);
	world.setVSDataBinding(renderEngine.getGeneralVSBinding());
	renderEngine.setSkybox(&ssbox);
	renderEngine.setFogColor(glm::vec3(0.0f));
	Gui& gui = Gui::getGui();

	// ui
	int window_w = 200;
	// int window_h = 60;
	int pos_x = 0.0f;
	int pos_y = 0.0f;
	int window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
										 | ImGuiWindowFlags_NoTitleBar;

	std::string worldName = "Earth";
	// float color[3] = {0.0f, 0.0f, 0.0f};
	glm::vec3 bgColor;

	float scale = 1.0f;
	gui.setGlobalFontScale(scale);
	gui.scaleAllSizes(scale);
	glm::vec3 vecOut = camera.getPosition();
	world.setGravity(glm::vec3(0.0f, -9.8, 0.0f));

	float in = 0.0f;
	float in2 = 0.0f;
	float in3 = 0.0f;
	float in4 =  9.8f;

	std::unique_ptr<Lamp> lamp1  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x, 0.0f, camera.getPosition().z));
	lamp1->setLightPosition(glm::vec3(camera.getPosition().x, lamp->getMaxBB().y-1.0f, camera.getPosition().z));
	lamp1->setDiffuse(glm::vec3(1.0f));
	lamp1->setSpecular(glm::vec3(1.0f));
	lamp1->setAttenuation(40.0f, 0.001f);
	entities.emplace_back(lamp1.release());
	debug_entity = entities.back().get();

	std::unique_ptr<Lamp> lamp2  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 20.0f, 0.0f, camera.getPosition().z));
	lamp2->setLightPosition(glm::vec3(camera.getPosition().x + 20.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z));
	lamp2->setDiffuse(glm::vec3(1.0f));
	lamp2->setSpecular(glm::vec3(1.0f));
	lamp2->setAttenuation(50.0f, 0.001f);
	entities.emplace_back(lamp2.release());

	std::unique_ptr<Lamp> lamp3  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 50.0f, 0.0f, camera.getPosition().z+20.0f));
	lamp3->setLightPosition(glm::vec3(camera.getPosition().x + 50.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z + 20.0f));
	lamp3->setDiffuse(glm::vec3(1.0f));
	lamp3->setSpecular(glm::vec3(1.0f));
	lamp3->setAttenuation(30.0f, 0.001f);
	entities.emplace_back(lamp3.release());

	std::unique_ptr<Lamp> lamp4  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 60.0f, 0.0f, camera.getPosition().z+20.0f));
	lamp4->setLightPosition(glm::vec3(camera.getPosition().x + 60.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z + 20.0f));
	lamp4->setDiffuse(glm::vec3(1.0f));
	lamp4->setSpecular(glm::vec3(1.0f));
	lamp4->setAttenuation(30.0f, 0.001f);
	entities.emplace_back(lamp4.release());

	std::unique_ptr<Lamp> lamp5  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 70.0f, 0.0f, camera.getPosition().z+20.0f));
	lamp5->setLightPosition(glm::vec3(camera.getPosition().x + 70.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z + 20.0f));
	lamp5->setDiffuse(glm::vec3(1.0f));
	lamp5->setSpecular(glm::vec3(1.0f));
	lamp5->setAttenuation(30.0f, 0.001f);
	entities.emplace_back(lamp5.release());

	lamp5  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 80.0f, 0.0f, camera.getPosition().z+20.0f));
	lamp5->setLightPosition(glm::vec3(camera.getPosition().x + 80.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z + 20.0f));
	lamp5->setDiffuse(glm::vec3(1.0f));
	lamp5->setSpecular(glm::vec3(1.0f));
	lamp5->setAttenuation(30.0f, 0.001f);
	entities.emplace_back(lamp5.release());


	lamp5  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 90.0f, 0.0f, camera.getPosition().z+20.0f));
	lamp5->setLightPosition(glm::vec3(camera.getPosition().x + 90.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z + 20.0f));
	lamp5->setDiffuse(glm::vec3(1.0f));
	lamp5->setSpecular(glm::vec3(1.0f));
	lamp5->setAttenuation(30.0f, 0.001f);
	entities.emplace_back(lamp5.release());


	lamp5  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 100.0f, 0.0f, camera.getPosition().z+20.0f));
	lamp5->setLightPosition(glm::vec3(camera.getPosition().x + 100.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z + 20.0f));
	lamp5->setDiffuse(glm::vec3(1.0f));
	lamp5->setSpecular(glm::vec3(1.0f));
	lamp5->setAttenuation(30.0f, 0.001f);
	entities.emplace_back(lamp5.release());


	lamp5  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 110.0f, 0.0f, camera.getPosition().z+20.0f));
	lamp5->setLightPosition(glm::vec3(camera.getPosition().x + 110.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z + 20.0f));
	lamp5->setDiffuse(glm::vec3(1.0f));
	lamp5->setSpecular(glm::vec3(1.0f));
	lamp5->setAttenuation(30.0f, 0.001f);
	entities.emplace_back(lamp5.release());


	lamp5  = std::make_unique<Lamp>(lamp, glm::vec3(camera.getPosition().x + 200.0f, 0.0f, camera.getPosition().z+20.0f));
	lamp5->setLightPosition(glm::vec3(camera.getPosition().x + 200.0f, lamp->getMaxBB().y-1.0f, camera.getPosition().z + 20.0f));
	lamp5->setDiffuse(glm::vec3(1.0f));
	lamp5->setSpecular(glm::vec3(1.0f));
	lamp5->setAttenuation(30.0f, 0.001f);
	entities.emplace_back(lamp5.release());

	renderEngine.setSceneAmbient(0.0f);

	// this required
	std::unique_ptr<PhysicsEntity> phy_entity = std::make_unique<PhysicsEntity>(cube, cubeShape, glm::vec3(10.0f), glm::vec3(0.0f), 1.0f);
	entities.emplace_back(phy_entity.release());

	FrameBuffer fbo(display.getScreenWidth(), display.getScreenHeight());
	fbo.use();
	fbo.setColorAttachment(FrameBuffer::TBUFFER);
	fbo.setDepthAttachment(FrameBuffer::RBUFFER);
	FrameBuffer fbo2(display.getScreenWidth(), display.getScreenHeight());
	fbo.useDefault();
	PostProcessor postProcessor;

	errorCheck();
	display.initLoop();
	while(!display.shouldClose()) {
		// process
		processInput(display, camera);
		if(!pause)
			world.processPhysics(display.getDelta());
		renderEngine.newFrame();

		PhysicsEntity* physicsEntity = static_cast<PhysicsEntity*>(entities.back().get());
		PhysicsBody* physicsBody = physicsEntity->getPhysicsBody();

		// GuiDraw::ShowDemoWindow();
		//----------------------------------------------------------

		// UI
		// window - left
		// Details window

		GuiDraw::Begin("Window", NULL, window_flags);
		GuiDraw::SetWindowPos(ImVec2(pos_x, pos_y));
		GuiDraw::SetWindowSize(ImVec2(window_w*scale, 0.0f));

		vecOut = camera.getPosition();
		GuiDraw::Text("Camera: %.2f %.2f %.2f", vecOut.x, vecOut.y, vecOut.z);
		GuiDraw::Separator();
		GuiDraw::Spacing();
		vecOut = physicsBody->getPosition();
		GuiDraw::Text("Pos: %.2f %.2f %.2f", vecOut.x, vecOut.y, vecOut.z);
		GuiDraw::Separator();
		vecOut = static_cast<RigidBody*>(physicsBody)->getLinearVelocity();
		GuiDraw::Text("Velocity : %.2f %.2f %.2f", vecOut.x, vecOut.y, vecOut.z);
		GuiDraw::Separator();
		GuiDraw::Text("Speed : %.2f", glm::length(vecOut));
		GuiDraw::Separator();
		GuiDraw::Text("Selected ball : %s", "Standard");
		GuiDraw::Separator();
		vecOut = world.getGravity();
		vecOut.y = (vecOut.y == -0.0f)? 0.0f : vecOut.y;
		GuiDraw::Text("Gravity : %.2f", -vecOut.y);
		GuiDraw::Separator();
		GuiDraw::Text("World Info : %s", worldName.c_str());
		GuiDraw::Separator();

		// PushItemWidth(-1) to align on the right-side.
		// if(GuiDraw::ColorEdit3("Background color", color)) {
		// 	// this code gets called if color value changes, so
    //   // the background color is upgraded automatically!
    //   bgColor.r = static_cast<u_int8_t>(color[0] * 255.f);
    //   bgColor.g = static_cast<u_int8_t>(color[1] * 255.f);
    //   bgColor.b = static_cast<u_int8_t>(color[2] * 255.f);
		// }

		// ImGui::InputText("Window title", windowTitle, 255);
   	// ImGui::Button("Update window title");
		GuiDraw::End();

		GuiDraw::Begin("", NULL, window_flags);
		GuiDraw::SetWindowPos(ImVec2(display.getScreenWidth() - (window_w+50.0f)*scale, pos_y));
		GuiDraw::SetWindowSize(ImVec2((window_w+50.0f)*scale, 0.0f));
		GuiDraw::Text("Select World");
		// if(GuiDraw::Button("Earth")){
		// 	space = false;
		// 	terrainGrid.getTerrain(0, 0)->getPhysicsBody()->addToWorld();
		// 	renderEngine.setSkybox(&sbox);
		// 	in4 = 9.8f;
		// 	world.setGravity(glm::vec3(0.0f, -9.8f, 0.0f));
		// 	// clear entites
		// 	entities.clear();
		// 	drawBlocks(block_pos, brick_mass);
		// 	worldName = "Earth";
		// };
		// GuiDraw::SameLine();
		// if(GuiDraw::Button("Space")){
		// 	space = true;
		// 	terrainGrid.getTerrain(0, 0)->getPhysicsBody()->removeFromWorld();
		// 	renderEngine.setSkybox(&ssbox);
		// 	world.setGravity(glm::vec3(0.0f));
		// 	in4 = 0.0f;
		// 	// clear entites
		// 	entities.clear();
		// 	drawBlocks(block_pos, brick_mass);
		// 	worldName = "Space";
		// };
		// GuiDraw::SameLine();
		// if(GuiDraw::Button("Moon")){

		// };

		GuiDraw::Separator();
		GuiDraw::InputFloat("Force ", &in);
		GuiDraw::InputFloat("Mass ", &in2);
		GuiDraw::InputFloat("Gravity ", &in4);
		GuiDraw::InputFloat("Friction" , &in3, 0.1, 1.0f);
		GuiDraw::InputFloat("Brick Mass", &brick_mass, 2.0f);
		//GuiDraw::InputFloat("");
		GuiDraw::Separator();

		GuiDraw::Text("Ball Type");
		if(GuiDraw::Button("Paper")){

		};
		GuiDraw::SameLine();
		if(GuiDraw::Button("Football")){
			steel = false;
		};
		GuiDraw::SameLine();
		if(GuiDraw::Button("Steel")){
			steel = true;
		};
		GuiDraw::Separator();
		GuiDraw::Indent(50.0f);
		if(GuiDraw::Button("Apply")) {
			force = in;
			mass = in2;
			friction = in3;
			world.setGravity(glm::vec3(0.0f, -in4, 0.0f));
		}
		GuiDraw::SameLine();
		if(GuiDraw::Button("Reset Block")) {
			entities.clear();
			drawBlocks(block_pos, brick_mass);
		}

		GuiDraw::End();

		//---------------------------------------------
		// process entities to render
		renderEngine.processEntity(entities);
		if(!space)
			renderEngine.processTerrain(terrainGrid.getTerrain(0, 0));

		fbo.use();
		renderEngine.render();
		fbo.useDefault();
		fbo.clearBuffer();

		postProcessor.applyVisualPerception(fbo.getColorID());
		glViewport(0, 0, display.getScreenWidth(), display.getScreenHeight());

		// world.debugDraw();
		// world.getDebugDrawer()->drawAabb(VEC3::glmToBt(debug_entity->getMinBB()), VEC3::glmToBt(debug_entity->getMaxBB()), btVector3(1.0f, 0.0f, 0.0f));
		// world.getDebugDrawer()->drawAabb(VEC3::glmToBt(entities.back()->getMinBB()), VEC3::glmToBt(entities.back()->getMaxBB()), btVector3(1.0f, 1.0f, 0.0f));
		// world.debugDraw();

		renderEngine.renderGui();

		errorCheck();
		// update
		display.update();
	}
	entities.clear();
}