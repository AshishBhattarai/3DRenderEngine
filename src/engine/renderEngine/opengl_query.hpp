#ifndef OPENGL_QUERY_HPP
#define OPENGL_QUERY_HPP

/* Represents gpu queries */

class OpenglQuery {
private:
	unsigned id;
	int type;
	bool first_run;
	int last_result;

public:
	OpenglQuery(int type);
	~OpenglQuery();

	void start();
	void end();

	bool isResultReady();
	bool isFirstRun();
	int getResult();
	unsigned getID();
	int getLastResult();
};

#endif