#include "opengl_query.hpp"

#include <glad/glad.h>


OpenglQuery::OpenglQuery(int type) :
	type(type),
	first_run(true),
	last_result(1) // initial frame is always 1
{
	glGenQueries(1, &id);
}

OpenglQuery::~OpenglQuery() {
	glDeleteQueries(1, &id);
}

void OpenglQuery::start() {
	glBeginQuery(type, id);
	if(first_run) first_run = false;
}

void OpenglQuery::end() {
	glEndQuery(type);
}

bool OpenglQuery::isResultReady() {
	int result;
	glGetQueryObjectiv(id, GL_QUERY_RESULT_AVAILABLE, &result);
	return result;
}

bool OpenglQuery::isFirstRun() {
	return first_run;
}

int OpenglQuery::getResult() {
	int result = 0;
	if(first_run) return 1; // return 1(true) if result queried before first run.
	glGetQueryObjectiv(id, GL_QUERY_RESULT, &result);
	last_result = result;
	return result;
}

unsigned OpenglQuery::getID() {
	return id;
}

int OpenglQuery::getLastResult() {
	return last_result;
}
