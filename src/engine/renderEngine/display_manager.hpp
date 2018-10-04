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
public:
	static constexpr int MAJOR_VERSION = 3;
	static constexpr int MINOR_VERSION = 3;

private:
	// typedefs
	using scrollCallback = std::function<void(float xoffset, float yoffset)>;
	using keyCallback = std::function<void(int key, int action, int mods)>;
	using mouseMovementCallback = std::function<void(float xoffset, float yoffset)>;
	using mouseButtonCallback = keyCallback;
	using charCallBack = std::function<void(unsigned int c)>;

	// members
	static constexpr int DEFAULT_WIDTH 	= 800;
	static constexpr int DEFAULT_HEIGHT =	600;

	int fps_cnt;
	float fps_time;
	double delta_time;
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
	std::vector<charCallBack> vec_charCallback;

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
	void addCharCallback(charCallBack cb) {
		vec_charCallback.push_back(cb);
	}

	void setCursorMode(int mode);
	int getCursorMode() const;
	void setCursor(GLFWcursor* cursor);
	void setCursorPos(float x, float y);
	void getCursorPos(float& x, float& y);
	void setShouldClose(bool value);
	void setClipboardText(const char* text);
	const char* getClipboardText() const;
	void getFramebufferSize(int& width, int& height);

	float getDelta() const {
		return delta;
	}

	bool shouldClose() const {
		return glfwWindowShouldClose(window);
	}

	bool isFocused() const {
		return glfwGetWindowAttrib(window, GLFW_FOCUSED);
	}

	int getKey(int key) const {
		return glfwGetKey(window, key);
	}

	int getMouseButton(int button) const {
		return glfwGetMouseButton(window, button);
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