#ifndef DISPLAYMANAGER_HPP
#define DISPLAYMANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <functional>

/**
 * A singleton class that manages display window
 */

class DisplayManager {

private:
	// typedefs
	using scrollCallback = std::function<void(float xoffset, float yoffset)>;
	using keyCallback = std::function<void(int key, int action, int mods)>;
	using mouseMovementCallback = std::function<void(float xoffset, float yoffset)>;
	using mouseButtonCallback = keyCallback;

	// members
	static constexpr int DEFAULT_WIDTH 	= 800;
	static constexpr int DEFAULT_HEIGHT =	600;

	static constexpr int MAJOR_VERSION = 3;
	static constexpr int MINOR_VERSION = 3;

	int fps_cnt;
	float fps_time;
	float delta_time;
	float delta;

	int screen_width;
	int screen_height;
	float mouse_lastx;
	float mouse_lasty;
	int cursor_mode; // normal, disabled, hidden

	bool display_failed;
	GLFWwindow* window;

	std::string window_title;
	std::string fps_str;

	std::vector<keyCallback> vec_keyCallback;
	std::vector<mouseMovementCallback> vec_mouseMovCallback;
	std::vector<mouseButtonCallback> vec_mouseBtnCallback;
	std::vector<scrollCallback> vec_scrollCallback;


	DisplayManager();
	~DisplayManager();
	void setupDisplay();
	void setupCallbacks();
	void calcFps();

public:
	// delete copy constructors
	DisplayManager(const DisplayManager&) = delete;
	DisplayManager& operator=(const DisplayManager&) = delete;

	static DisplayManager& getInstance() {
		static DisplayManager instance;
		return instance;
	}

	// must be called before loop
	void initLoop();

	// update display manager, must be called at the end of the game loop
	void update();

	// add to callbacks
	void addKeyCallback(keyCallback cb) {
		vec_keyCallback.push_back(cb);
	}
	void addMouseMovementCallback(mouseMovementCallback cb) {
		vec_mouseMovCallback.push_back(cb);
	}
	void addMouseButtonCallback(mouseButtonCallback cb) {
		vec_mouseBtnCallback.push_back(cb);
	}
	void addScrollCallback(scrollCallback cb) {
		vec_scrollCallback.push_back(cb);
	}

	void setCursorMode(int mode);

	void setShouldClose(bool value) {
		glfwSetWindowShouldClose(window, value);
	}

	float getDelta() const {
		return delta;
	}

	bool shouldClose() const {
		return glfwWindowShouldClose(window);
	}

	int getKey(int key) const {
		return glfwGetKey(window, key);
	}

	bool isFailed() const {
		return display_failed;
	}

	int getScreenWidth() {
		return screen_width;
	}

	int getScreenHeight() {
		return screen_height;
	}
};

#endif