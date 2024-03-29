#pragma once

#include <sita_core.h>

#define SDL_MAIN_HANDLED
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui.h"
#include "imgui_impl_sdl2.h"

#include "ImGuiHelper.h"
#include <stdio.h>
#include <SDL.h>

namespace sita {

class ImGuiApp {
public:
	virtual ~ImGuiApp();

	bool getInputKey(SDL_Keycode key) {
		auto s = SDL_GetScancodeFromKey(key);
		return ImGui::GetIO().KeysDown[s];
	}

	void setTitle(StrView title);

	void run();
	virtual void onInit() {}
	virtual void onUpdate(float deltaTime) {}
	virtual void onEvent(SDL_Event& ev) {};

private:
	class Impl;
	Impl* _impl = nullptr;
};

} // namespace