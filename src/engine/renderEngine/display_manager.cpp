#include "display_manager.hpp"

#include "utils/slogger.hpp"

DisplayManager::DisplayManager() :
		fps_cnt(0),
		fps_time(0.0f),
		delta_time(0.0f),
		delta(0.0f),
		screen_width(DEFAULT_WIDTH),
		screen_height(DEFAULT_HEIGHT),
		mouse_lastx(screen_width/2),
		mouse_lasty(screen_height/2),
		cursor_mode(GLFW_CURSOR_HIDDEN),
		display_failed(false),
		window(nullptr),
		window_title("Engine1")
{
	setupDisplay();
	setupCallbacks();
}

DisplayManager::~DisplayManager() {
	if(window)
		glfwDestroyWindow(window);
	glfwTerminate();
}

void DisplayManager::setupDisplay() {
	//error callback
	glfwSetErrorCallback([] (int ec, const char* desc) {
		SLOG("Error: ", desc, " Error code: ", ec);
	});

	if(!glfwInit()) {
		display_failed = true;
		return;
	}

	// set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // disable resize

	window = glfwCreateWindow(screen_width, screen_height, window_title.c_str(),
			NULL, NULL);
	if(!window) {
		display_failed = true;
		return;
	}

	// set opengl context
	glfwMakeContextCurrent(window);

	// init glad
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		display_failed = true;
		SLOG("Failed to init glad");
		return;
	}

	// set view port
	glViewport(0, 0, screen_width, screen_height);

	// store DisplayManager(this) as user pointer
	glfwSetWindowUserPointer(window, this);

	// change view port on resize
	glfwSetFramebufferSizeCallback(window, [] (GLFWwindow* window,
			int width, int height) {
		// get user pointer
		auto self = static_cast<DisplayManager*>(glfwGetWindowUserPointer(window));

		self->screen_width = width;
		self->screen_height = height;
		glViewport(0, 0, width, height);
		//TODO: update projection matrix
	});

	// set cursor mode
	glfwSetInputMode(window, GLFW_CURSOR, cursor_mode);
}

void DisplayManager::setupCallbacks() {

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int /* scancode */,
			int action, int mods) {
		// get this pointer
		auto self = static_cast<DisplayManager*>(glfwGetWindowUserPointer(window));
		// handel all the key callbacks
		for(auto cb : self->vec_keyCallback)
			cb(key, action, mods);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window,
			double xpos, double ypos) {
		// this
		auto self = static_cast<DisplayManager*>(glfwGetWindowUserPointer(window));

		// calculate movement(offset)
		float xoffset = (float)xpos - self->mouse_lastx;
		float yoffset = (float)ypos - self->mouse_lasty;
		self->mouse_lastx = (float)xpos;
		self->mouse_lasty = (float)ypos;

		// callbacks
		for(auto cb : self->vec_mouseMovCallback)
			cb(xoffset, yoffset);

	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window,
			int button, int action, int mods) {
		// this
		auto self = static_cast<DisplayManager*>(glfwGetWindowUserPointer(window));
		// callbacks
		for(auto cb : self->vec_mouseBtnCallback)
			cb(button, action, mods);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window,
			double xoffset, double yoffset) {
		// this
		auto self = static_cast<DisplayManager*>(glfwGetWindowUserPointer(window));
		// callbacks
		for(auto cb : self->vec_scrollCallback) {
			cb((float)xoffset, (float)yoffset);
		}
	});

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int c){
		auto self = static_cast<DisplayManager*>(glfwGetWindowUserPointer(window));
		// callbacks
		for(auto cb : self->vec_charCallback) {
			cb(c);
		}
	});
}

void DisplayManager::calcFps() {
	if((float) glfwGetTime() - fps_time >= 1) {
		fps_str = std::to_string(fps_cnt);
		glfwSetWindowTitle(window, (window_title + " " + fps_str).c_str());
		fps_cnt = 0;
		fps_time = (float) glfwGetTime();
	}
	++fps_cnt;
}

void DisplayManager::initLoop() {
	glfwSwapInterval(1);
	glfwSetCursorPos(window, mouse_lastx, mouse_lasty);
	delta_time = glfwGetTime();
	fps_time = (float)delta_time;
}

void DisplayManager::update() {
	// calc delta
	double current_time = glfwGetTime();
	delta = (float)(current_time - delta_time);
	delta_time = current_time;

	calcFps();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

// disable enable cursor
void DisplayManager::setCursorMode(int mode) {
	if (cursor_mode == mode) return;
	else	{
		glfwSetInputMode(window, GLFW_CURSOR, mode);
		cursor_mode = mode;
	}
}

int DisplayManager::getCursorMode() const {
	return cursor_mode;
}

void DisplayManager::setCursor(GLFWcursor* cursor) {
	glfwSetCursor(window, cursor);
}

void DisplayManager::setShouldClose(bool value) {
	glfwSetWindowShouldClose(window, value);
}

void DisplayManager::setCursorPos(float x, float y) {
	glfwSetCursorPos(window, (double)x, (double)y);
}
void DisplayManager::getCursorPos(float& x, float& y) {
	double d_x, d_y;
	glfwGetCursorPos(window, &d_x, &d_y);
	x = (float) d_x;
	y = (float) d_y;
}

void DisplayManager::setClipboardText(const char* text) {
	glfwSetClipboardString(window, text);
}

const char* DisplayManager::getClipboardText() const {
	return glfwGetClipboardString(window);
}

void DisplayManager::getFramebufferSize(int& width, int& height) {
	glfwGetFramebufferSize(window, &width, &height);
}

float DisplayManager::getTimeSec() const {
	return (float)(glfwGetTime()*1000.0f);
}