#include "gui_renderer.hpp"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "display_manager.hpp"

GuiRenderer::GuiRenderer(GuiShader& shader) :
	vao(0), vbo(0), ebo(0), font_texture(0),
	shader(&shader)
{
	initBuffers();
	buildFonts();
}

GuiRenderer::~GuiRenderer() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteTextures(1, &font_texture);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->TexID = 0;
}

void GuiRenderer::initBuffers() {
	// generate
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenTextures(1, &font_texture);

	// setup attributes
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // uv
	glEnableVertexAttribArray(2); // color

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, col));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0); // pos
	glDisableVertexAttribArray(1); // uv
	glDisableVertexAttribArray(2); // color
}

void GuiRenderer::buildFonts() {
	// Build texture atlas for fonts
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* data; // pixel data
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
	// load into opengl
	glBindTexture(GL_TEXTURE_2D, font_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// store the id
	io.Fonts->TexID =(ImTextureID)(intptr_t)font_texture;
}

void GuiRenderer::drawCmdList(const ImDrawList* cmd_list) {
	// draw ui
	const ImDrawIdx* idx_buffer_offfset = 0; // vertex index offset
	// vertex data
	glBufferData(GL_ARRAY_BUFFER, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
	// vertex index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

	// loop through commands
	for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i) {
		const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i]; // single command
		if(pcmd->UserCallback)
			pcmd->UserCallback(cmd_list, pcmd); // user custom draw function callback - ImDrawList::AddCallback
		else {
			//	clipping window, bound
			ImVec4 clip_rect = ImVec4(pcmd->ClipRect.x - display_pos.x, pcmd->ClipRect.y - display_pos.y, pcmd->ClipRect.z - display_pos.x, pcmd->ClipRect.w - display_pos.y);
			// check if clipping bound is less thand frame buffer
      if(clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f) {
        // Apply scissor/clipping rectangle
        glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glDrawElements(GL_TRIANGLES, pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offfset);
			}
		}
		// increase the offset
		idx_buffer_offfset += pcmd->ElemCount;
	}
}

void GuiRenderer::render() {
	// get render data
	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();

	// render process
	ImGuiIO& io = ImGui::GetIO();
	fb_width = (int)(draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
  fb_height = (int)(draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);
	display_pos = draw_data->DisplayPos;

	// scale for framebuffer
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);

	// bind vao, vbo, ebo
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	shader->start();

	// loop through command lists
	for(int n = 0; n < draw_data->CmdListsCount; ++n) {
		drawCmdList(draw_data->CmdLists[n]);
	}

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
}