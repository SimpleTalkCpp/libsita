#include <sita_imgui.h>

namespace sita {

class MyApp : public ImGuiApp {
public:
	int port = 30000;
	char hostname[256] = "localhost";

	bool isConnected = false;
	bool isHost() const { return false; }

	virtual void onInit() override {
		setTitle("sita_net_example_003");
	}

	void updateUI() {
		if (!isConnected) {
			ImGui::InputText("", hostname, sizeof(hostname));
			ImGui::InputInt("", &port);

			if (ImGui::Button("Host")) {
				try {
				} catch (...) {
					SITA_LOG("error listen");
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Join")) {
				try {
				} catch (...) {
					SITA_LOG("error connect");
				}
			}
			return;
		}

		ImGui::Text("Connected");
		if (ImGui::Button("Disconnect")) {
			isConnected = false;
		}
	}

	virtual void onUpdate(float deltaTime) override {
		ImGui::Begin("UI");
		updateUI();
		ImGui::End();
	}
};

} // namespace

int main()
{
	srand(time(nullptr));

	using namespace sita;
	MyApp app;
	app.run();
    return 0;
}