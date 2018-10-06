#ifndef GUI_HPP
#define GUI_HPP

#include <imgui/imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// creats imgui context

namespace GuiDraw = ImGui;
class Gui {
private:
	static constexpr int MAX_MOUSE_BUTTONS = 5;

	GLFWcursor* mouseCursors[ImGuiMouseCursor_Count_]; // mousecursor mods (draw arrow, loading, ..)
	bool mouseJustPressed[MAX_MOUSE_BUTTONS]; // mousebutton states

	Gui();
	~Gui();

	void updateMousePosAndButtons();
	void updateMouseCursor();

public:
	static Gui& getGui() {
		static Gui instance;
		return instance;
	}

	Gui(const Gui&) = delete;
	Gui& operator=(const Gui&) = delete;

	void newFrame();

	void setGlobalFontScale(float scale);

	void scaleAllSizes(float scale);
};

#endif