#include <sita_net_common.h>
#include <sita_imgui.h>

namespace sita {

class MyNetEngine : public NetEngine {
public:
	
};

class MyApp : public ImGuiApp {
public:
	int port = 30000;
	char hostname[256] = "localhost";

	int testConnCount = 10;

	bool isConnected = false;
	bool isHost() const { return false; }

	MyNetEngine _netEngine;

	virtual void onInit() override {
		setTitle("sita_net_example_003");
	}

	void updateUI() {
		if (!isConnected) {
			ImGui::InputText("Hostname", hostname, sizeof(hostname));
			ImGui::InputInt("Port", &port);

			{
				ImGui::BeginGroup();

				ImGui::LabelText("Listen Count", "%u", _netEngine.listenCount());
				if (ImGui::Button("Listen")) {
					try {
						_netEngine.listen(hostname, port);
					} catch (...) {
						SITA_LOG("error listen");
					}
				}

				ImGui::SameLine();
				if (ImGui::Button("Stop Listen")) {
					_netEngine.stopListen();
				}
				ImGui::EndGroup();
			}

			ImGui::LabelText("Connected Count", "%u", _netEngine.connectedCount());
			
			ImGui::InputInt("Test Connection Count", &testConnCount);
			if (testConnCount < 1) testConnCount = 1;

			if (ImGui::Button("Connect")) {
				try {
					_netEngine.connect(hostname, static_cast<u16>(port));
				} catch (...) {
					SITA_LOG("error connect");
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Disconnect All")) {
				_netEngine.disconnectAll();
			}

			return;
		}

		ImGui::Text("Connected");
		if (ImGui::Button("Disconnect")) {
			isConnected = false;
		}
	}

	virtual void onUpdate(float deltaTime) override {
		_netEngine.update();

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