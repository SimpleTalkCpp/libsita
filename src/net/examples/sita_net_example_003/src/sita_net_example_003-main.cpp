#include <sita_net_common.h>
#include <sita_imgui.h>

namespace sita {

enum class MyPacketCmd : NEPacketCmd {
	Hello,
	Test,
};

#define MyPacket_Define(T) \
	virtual void onWrite(BinSerializer&   se) { io(se); } \
	virtual void onRead (BinDeserializer& se) { io(se); } \
public: \
	MyPacket_##T() : MyPacket(Cmd::T) {} \

//-------

class MyPacket : public NEPacket {
public:
	using Cmd = MyPacketCmd;

	MyPacket(MyPacketCmd cmd) : NEPacket(enumInt(cmd)) {}

};

class MyPacket_Hello : public MyPacket {
	MyPacket_Define(Hello)
public:
	String_<32> msg;

	template<class SE>
	void io(SE& se) {
		se << msg;
	}
};

class MyPacket_Test : public MyPacket {
	MyPacket_Define(Test)
public:
	u32 seq = 0;
	static const u32 kDataSize = 200;
	Vector_<u32, kDataSize>	data;

	void initData(u32 seq_) {
		seq = seq_;
		data.resize(kDataSize);
		for (u32 i = 0; i < kDataSize; i++) {
			data[i] = i + seq;
		}
	}

	bool validateData() {
		if (data.size() != kDataSize) {
			SITA_ASSERT(false);
			return false;
		}

		for (u32 i = 0; i < kDataSize; i++) {
			if (data[i] != i + seq) {
				SITA_ASSERT(false);
				return false;
			}
		}
		return true;
	}

	template<class SE>
	void io(SE& se) {
		se << seq << data;
	}
};

class MyNetEngine : public NetEngine {
public:
	virtual void onAccept(NESocket* s) override {
//		SITA_LOG("onAccept");
	}

	virtual void onConnect(NESocket* s) override {
//		SITA_LOG("onConnect");

		MyPacket_Hello pkt;
		pkt.msg = "hello";
		sendPacket(s, pkt);
	}

	virtual void onDisconnect(NESocket* s) override {
		SITA_LOG("onDisconnect");
	}

	virtual void onError(NESocket* s, const Error& err) override {
		SITA_LOG("onError");
	}


	virtual void onRecvPacket(NESocket* s, const NEPacketHeader& hdr, Span<u8> data) override {
		using Cmd = MyPacketCmd;
		auto cmd = static_cast<Cmd>(hdr.cmd);

		#define RECV_PACKET_CASE(T) \
			case Cmd::T: { \
				MyPacket_##T pkt; \
				pkt.readFromBuffer(data); \
				_onRecvPacket(s, pkt); \
			} break; \
		//-----

		switch (cmd) {
			RECV_PACKET_CASE(Hello) \
			RECV_PACKET_CASE(Test) \
		}

		#undef RECV_PACKET_CASE
	}

	void _onRecvPacket(NESocket* s, MyPacket_Hello& pkt) {
//		SITA_LOG("MyPacket_Hello");
		MyPacket_Test outPkt;
		outPkt.initData(10);
		sendPacket(s, outPkt);
	}

	void _onRecvPacket(NESocket* s, MyPacket_Test& pkt) {
//		SITA_LOG("MyPacket_Test");
		if (!pkt.validateData()) {
			SITA_ASSERT(false);
		}

		pkt.initData(pkt.seq + 1);
		sendPacket(s, pkt);
	}

};

class MyApp : public ImGuiApp {
public:
	int port = 30000;
	char hostname[256] = "localhost";

	int testConnCount = 1000;

	bool isHost() const { return false; }

	MyNetEngine _netEngine;

	virtual void onInit() override {
		setTitle("sita_net_example_003");
	}

	void updateUI() {
		{
			auto& rs = _netEngine.recvStatis();
			auto& ss = _netEngine.recvStatis();

			ImGui::Text("statistics:\n"
						"  recv %10llu kB, total %10llu kB\n"
						"  send %10llu kB, total %10llu kB\n"
						, rs.lastSecond / 1024, rs.total / 1024
						, ss.lastSecond / 1024, rs.total / 1024
						);
		}
		ImGui::Spacing();

		ImGui::InputText("Hostname", hostname, sizeof(hostname));
		ImGui::InputInt("Port", &port);

		{
			ImGui::BeginGroup();

			ImGui::LabelText("Listen Count", "%u", _netEngine.listenCount());
			if (ImGui::Button("Listen")) {
				try {
					_netEngine.listen(hostname, port, testConnCount * 2);
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
				for (int i = 0; i < testConnCount; i++) {
					_netEngine.connect(hostname, static_cast<u16>(port));
				}
			} catch (...) {
				SITA_LOG("error connect");
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Disconnect All")) {
			_netEngine.disconnectAll();
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