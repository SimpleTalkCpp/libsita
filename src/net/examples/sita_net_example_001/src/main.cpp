#include <sita/sita_core.h>
#include "GameApp.h"

namespace sita {

class MyApp : public GameApp {
public:
	ImVec2 playerPos {400,300};

	virtual void onUpdate(float deltaTime) {

		ImVec2 dir {0,0};
		float speed = 200;

		if (getInputKey(SDLK_w)) dir.y -= 1;
		if (getInputKey(SDLK_s)) dir.y += 1;
		if (getInputKey(SDLK_a)) dir.x -= 1;
		if (getInputKey(SDLK_d)) dir.x += 1;

		playerPos.x += dir.x * deltaTime * speed;
		playerPos.y += dir.y * deltaTime * speed;

//		auto* drawList = ImGui::GetWindowDrawList();
//		auto* drawList = ImGui::GetForegroundDrawList();
		auto* drawList = ImGui::GetBackgroundDrawList();

		drawList->AddTriangleFilled(ImVec2(playerPos.x,      playerPos.y - 12), 
									ImVec2(playerPos.x - 12, playerPos.y + 12),
									ImVec2(playerPos.x + 12, playerPos.y + 12), 
									ImColor(255, 0, 0));
	}

	virtual void onEvent(SDL_Event& ev) {
		/*
		switch (ev.type) 
		{
			case SDL_KEYDOWN: {
				auto key = ev.key.keysym.sym;
				switch (key) {
					case SDLK_w: playerPos.y -= 10; break;
					case SDLK_a: playerPos.x -= 10; break;
					case SDLK_s: playerPos.y += 10; break;
					case SDLK_d: playerPos.x += 10; break;
				}
			} break;
		}
		*/
	};
};

} // namespace

int main()
{
	using namespace sita;
	MyApp app;
	app.run();
    return 0;
}