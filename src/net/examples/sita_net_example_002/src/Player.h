#pragma once

#include "pch.h"

namespace sita {

class Player {
public:

	void draw();
	void move(const ImVec2& v);
	void setPos(const ImVec2& v) { _pos = v; }
private:
	ImVec2 _pos;
};

}