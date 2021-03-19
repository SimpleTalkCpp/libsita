#pragma once

#include "pch.h"

namespace sita {

class GameApp {
public:
	virtual ~GameApp();

	bool getInputKey(SDL_Keycode key) {
		auto s = SDL_GetScancodeFromKey(key);
		return ImGui::GetIO().KeysDown[s];
	}

	void run();

	virtual void onInit() {}
	virtual void onDeinit() {}
	virtual void onUpdate(float deltaTime) {}
	virtual void onEvent(SDL_Event& ev) {}

	ImVec2 screenSize() const;
};


} // namespace