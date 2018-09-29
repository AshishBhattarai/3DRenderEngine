#ifndef DEBUG_DRAWER_HPP
#define DEBUG_DRAWER_HPP

#include <bullet/LinearMath/btIDebugDraw.h>

#include "shader/debug_physics_shader.hpp"
#include "shader/shader_config.hpp"

class DebugDrawer : public btIDebugDraw {
private:
	// this draws two point at a give frame so - 3(pos) + 3(color) x 2
	static constexpr float DATA_SIZE = 12;
	enum Mode {
		LINES		=	 	0,
		POINTS	=		1
	};

	GLuint VBO, VAO;
	DebugPhysicsShader shader;
	// color for contact points
	btVector3 pointsColor = btVector3(0.0f, 0.0f, 1.0f);

	void allocateBuffer();
	void draw(const btVector3& from, const btVector3& to, const btVector3& color, Mode mode);

public:
	DebugDrawer();
	virtual ~DebugDrawer();

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB,
		btScalar distance, int lifeTime, const btVector3& color);

	void reportErrorWarning(const char *) {}
	void draw3dText(const btVector3 &, const char *) {}
	void setDebugMode(int) {}

	int getDebugMode() const {
		return (btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_FastWireframe |
						btIDebugDraw::DBG_DrawAabb |  btIDebugDraw:: DBG_DrawContactPoints);
	}

	void setVSDataBinding(u_int point) {
		shader.setUniformBlockBinding(ShaderConfig::GeneralVSData, point);
	}

};


#endif