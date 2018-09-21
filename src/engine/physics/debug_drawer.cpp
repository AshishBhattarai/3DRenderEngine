#include "debug_drawer.hpp"

void DebugDrawer::allocateBuffer() {
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, DATA_SIZE*sizeof(float), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

DebugDrawer::DebugDrawer() :
	shader()
{
	allocateBuffer();
}

DebugDrawer::~DebugDrawer() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void DebugDrawer::draw(const btVector3& from, const btVector3& to, const btVector3& color, Mode mode) {
	// vertex data
	float points[12];

	points[0] = from.x();
	points[1] = from.y();
	points[2] = from.z();
	points[3] = color.x();
	points[4] = color.y();
	points[5] = color.z();

	points[6] = to.x();
	points[7] = to.y();
	points[8] = to.z();
	points[9] = color.x();
	points[10] = color.y();
	points[11] = color.z();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, DATA_SIZE*sizeof(float), points);

	shader.start();
	shader.setIsContactPoint((int)mode);
	glBindVertexArray(VAO);
	if(mode == LINES)
		glDrawArrays(GL_LINES, 0, 2);
	else {
		glEnable(GL_PROGRAM_POINT_SIZE);
		glDrawArrays(GL_POINTS, 0, 2);
	}
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	draw(from, to, color, LINES);
}

void DebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB,
		btScalar distance, int lifeTime, const btVector3& color) {

	btVector3 to = pointOnB + normalOnB*distance;
	draw(pointOnB, to, pointsColor, POINTS);
}