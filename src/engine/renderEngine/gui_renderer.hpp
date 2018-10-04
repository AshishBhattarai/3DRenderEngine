#ifndef GUI_RENDERER_HPP
#define GUI_RENDERER_HPP

#include <imgui/imgui.h>

#include "shader/gui_shader.hpp"

class GuiRenderer {
private:
	// render data
	GLuint vao, vbo, ebo, font_texture;
	GuiShader* shader;

	// framebuffer size
	int fb_width;
	int fb_height;

	/**
	 * Starting position of current viewport.
	 * Display position will always be (0, 0). Except for multiple view ports.
	**/
	ImVec2 display_pos;

	void initBuffers();
	void buildFonts();

	/**
	* Draw Command List
	* This is the low-level list of polygons that ImGui functions are filling.
	* At the end of the frame, all command lists are passed to your renderer.
	*
	* Draws vertex data on the given cmd_list(command list).
	*
	**/
	void drawCmdList(const ImDrawList* cmd_list);

public:
	GuiRenderer(GuiShader& shader);
	~GuiRenderer();

	void render();
};

#endif