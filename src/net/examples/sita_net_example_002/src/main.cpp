#include "GameApp.h"
#include "Player.h"

namespace sita {

class MyApp : public GameApp {
public:
	std::vector<Player> playerList;

	virtual void onInit() override {
		playerList.resize(1);
		auto& player = playerList[0];
		player.setPos({600, 200});
	}

	class UIPanel {
	public:
		char hostname[256] = "localhost";

		void update() {
			ImGui::Begin("MyUI");

			ImGui::Text("This is some useful text.");

			ImGui::InputText("", hostname, sizeof(hostname));

			if (ImGui::Button("Host")) {
			}

			ImGui::SameLine();

			if (ImGui::Button("Join")) {
			}

			ImGui::End();
		}
	};

	UIPanel panel;

	void updateInput(float deltaTime) {
		ImVec2 dir {0,0};
		float speed = 200;

		if (getInputKey(SDLK_w)) dir.y -= 1;
		if (getInputKey(SDLK_s)) dir.y += 1;
		if (getInputKey(SDLK_a)) dir.x -= 1;
		if (getInputKey(SDLK_d)) dir.x += 1;

		auto& player = playerList[0];
		player.move(dir * deltaTime * speed);
	}

	virtual void onUpdate(float deltaTime) override {
		panel.update();
		updateInput(deltaTime);

		for (auto& p : playerList) {
			p.draw();
		}
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