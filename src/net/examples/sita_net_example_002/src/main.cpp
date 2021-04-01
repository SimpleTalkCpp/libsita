#include "GameApp.h"
#include "Player.h"

namespace sita {

class MyApp : public GameApp {
public:
	Player player;
	Player anotherPlayer;

	Socket _listenSock;
	Socket _sock;

	String _sockBuf;

	int port = 30000;
	char hostname[256] = "localhost";

	bool isConnected = false;
	bool isHost() const { return _listenSock.isValid(); }

	virtual void onInit() override {
		player.setActive(true);
		player.setPos({
			static_cast<float>(rand() % 1000 + 100),
			static_cast<float>(rand() % 400  + 100)
		});

		player.setColor({
			rand() % 255,
			rand() % 255,
			rand() % 255
		});
	}

	void setDisconnected() {
		_listenSock.close();
		_sock.close();
		isConnected = false;
	}

	void updateUI() {
		if (!isConnected) {
			ImGui::InputText("", hostname, sizeof(hostname));
			ImGui::InputInt("", &port);

			if (ImGui::Button("Host")) {
				try {
					_listenSock.createTCP();
					_listenSock.setReuseAddr(true);
					_listenSock.bind(hostname, static_cast<uint16_t>(port));
					_listenSock.listen();
					_listenSock.accept(_sock);

					auto col = player.color().Value;
					_sock.send(Fmt("color {} {} {}\n", col.x, col.y, col.z));

					isConnected = true;
				} catch (const Error&) {
					SITA_LOG("error listen");
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Join")) {
				try {
					_sock.createTCP();
					_sock.connect(hostname, static_cast<uint16_t>(port));

					auto col = player.color().Value;
					_sock.send(Fmt("color {} {} {}\n", col.x, col.y, col.z));

					isConnected = true;
				} catch (const Error&) {
					SITA_LOG("error connect");
				}
			}
			return;
		}

		ImGui::Text("Connected");
		if (ImGui::Button("Disconnect")) {
			_sock.close();
			isConnected = false;
		}
	}

	void updateInput(float deltaTime) {
		ImVec2 dir {0,0};
		float speed = 300;

		if (getInputKey(SDLK_w)) dir.y -= 1;
		if (getInputKey(SDLK_s)) dir.y += 1;
		if (getInputKey(SDLK_a)) dir.x -= 1;
		if (getInputKey(SDLK_d)) dir.x += 1;

		player.move(dir * deltaTime * speed);
	}

	void handleCmd(const char* cmdline) {
//		SITA_LOG("cmd = {}", cmdline);

		char cmd[100+1];
		if (1 != sscanf(_sockBuf.c_str(), "%100s", cmd))
			return;
		cmd[100] = 0;

		if (0 == strcmp(cmd, "move")) {
			ImVec2 pos;
			if (3 == sscanf(_sockBuf.c_str(), "%100s %f %f", cmd, &pos.x, &pos.y)) {
				anotherPlayer.setPos(pos);
			}
			return;
		}

		if (0 == strcmp(cmd, "color")) {
			float r,g,b;
			if (4 == sscanf(_sockBuf.c_str(), "%100s %f %f %f", cmd, &r, &g, &b)) {
				anotherPlayer.setColor({r,g,b});
			}
			return;
		}
	}

	void updateNetwork() {
		if (!_sock.isValid()) {
			anotherPlayer.setActive(false);
			return;
		}

		// send my player
		{
			auto pos = player.pos();
			int ret = _sock.send(fmt::format("move {} {}\n", pos.x, pos.y).c_str());
			if (ret <= 0) {
				setDisconnected();
			}
		}

		// recv another player
		{
			anotherPlayer.setActive(true);

			size_t n = _sock.availableBytesToRead();
			if (n <= 0) return;

			int ret = _sock.appendRecv(_sockBuf, n, 0);
			if (ret <= 0) {
				setDisconnected();
			}
		
			if (_sockBuf.size()) {
				char* s = _sockBuf.data();
				char* end = s + _sockBuf.size();

				for (; s < end; s++) {
					auto* p = strchr(s, '\n');
					if (!p) break;

					*p = 0;
					handleCmd(s);
					s = p + 1;
				}

				String tmp;
				if (s < end) {
					tmp = s;
				}

				std::swap(_sockBuf, tmp);
			}
		}
	}

	virtual void onUpdate(float deltaTime) override {
		ImGui::Begin("UI");
		updateUI();
		ImGui::End();

		updateNetwork();

		updateInput(deltaTime);

		player.draw();
		anotherPlayer.draw();
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