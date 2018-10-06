#include "gui.hpp"

#include "renderEngine/display_manager.hpp"
#include "utils/slogger.hpp"

Gui::Gui():
	mouseCursors{nullptr},
	mouseJustPressed{false, false, false, false, false}
{
	DisplayManager& display = DisplayManager::getInstance();

	// create imgui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//io backends
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// keyboard mapping to glfw
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	// clipboard setter and getter
	io.SetClipboardTextFn = [](void* user_data, const char* text) {
		DisplayManager* display = static_cast<DisplayManager*>(user_data);
		display->setClipboardText(text);
	};
	io.GetClipboardTextFn = [](void* user_data)->const char*{
		DisplayManager* display = static_cast<DisplayManager*>(user_data);
		return display->getClipboardText();
	};
	io.ClipboardUserData = &display;

	// cursor modes
	mouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  mouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
  mouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // GLFW doesn't have this.
  mouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
  mouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  mouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // GLFW doesn't have this.
  mouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // GLFW doesn't have this.
  mouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);


	// callbacks
	display.addKeyCallback([&io](int key, int action, int /* mods */){
		if(action == GLFW_PRESS)
			io.KeysDown[key] = true;
		if(action == GLFW_RELEASE)
			io.KeysDown[key] = false;

		// extra for left right keys
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	});

	display.addMouseButtonCallback([&io, &mouseJustPressed = mouseJustPressed]
		(int button, int action, int /* mods */)
	{
		if(action == GLFW_PRESS && button >= 0 && button < MAX_MOUSE_BUTTONS)
			mouseJustPressed[button] = true;
	});

	display.addScrollCallback([&io](float xoffset, float yoffset){
		io.MouseWheelH += xoffset;
		io.MouseWheel += yoffset;
	});

	display.addCharCallback([&io](unsigned int c){
		if(c > 0 && c < 0x10000)
			io.AddInputCharacter((unsigned short)c);
	});

	// setup style
	ImGui::CreateContext();
	// ImGui::StyleColorsClassic();
	// TTF fonts
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
}

Gui::~Gui() {
	// cleanup
	for(auto& cursor : mouseCursors) {
		glfwDestroyCursor(cursor);
		cursor = nullptr;
	}
	ImGui::DestroyContext();
}

void Gui::updateMousePosAndButtons() {
	DisplayManager& display = DisplayManager::getInstance();
	ImGuiIO& io = ImGui::GetIO();

	// update mouse buttons
	for(int i = 0; i < IM_ARRAYSIZE(io.MouseDown); ++i) {
		io.MouseDown[i] = mouseJustPressed[i] || display.getMouseButton(i) != 0;
		mouseJustPressed[i] = false;
	}

	// update mouse position
	const ImVec2 mouse_pos = io.MousePos;
	const bool focused = display.isFocused();

	if(focused) {
		if(io.WantSetMousePos)
			display.setCursorPos(mouse_pos.x, mouse_pos.y);
		else {
			float x, y;
			display.getCursorPos(x, y);
			io.MousePos = ImVec2(x, y);
		}
	}
}

void Gui::updateMouseCursor() {
	DisplayManager& display = DisplayManager::getInstance();
	ImGuiIO& io = ImGui::GetIO();

	// update mouse cursor
	if((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || display.getCursorMode() == GLFW_CURSOR_DISABLED)
		return;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();

	if(imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
		// hide os cursor if custom cursor or no cursor
		display.setCursorMode(GLFW_CURSOR_HIDDEN);
	} else {
		// show os mouse cursor
		// set cursor scale, arrow, drag... if exists
		display.setCursor(mouseCursors[imgui_cursor]? mouseCursors[imgui_cursor] : mouseCursors[ImGuiMouseCursor_Arrow]);
		display.setCursorMode(GLFW_CURSOR_NORMAL);
	}
}

void Gui::newFrame() {
	// font atlas needs to be build before calling newFrame
	DisplayManager& display = DisplayManager::getInstance();

	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt());

	// set up display size
	int fb_width, fb_height;
	float s_width = (float)display.getScreenWidth();
	float s_height = (float)display.getScreenHeight();
	display.getFramebufferSize(fb_width, fb_height);
	io.DisplaySize = ImVec2(s_width, s_height);
	io.DisplayFramebufferScale = ImVec2(fb_width > 0 ? (s_width/fb_width) : 0, fb_height > 0 ? (s_height/fb_height) : 0);
	io.DeltaTime = display.getDelta();

	updateMousePosAndButtons();
	updateMouseCursor();

	ImGui::NewFrame();
}

void Gui::setGlobalFontScale(float scale) {
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = scale;
}

void Gui::scaleAllSizes(float scale) {
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(scale);
}