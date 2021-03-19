#pragma once

#include "pch.h"

namespace sita {

class Player {
public:
	bool isActive() const { return _active; }
	void setActive(bool b) { _active = b; }

	void draw();
	void move(const ImVec2& v);
	void setPos(const ImVec2& v) { _pos = v; }
	const ImVec2& pos() const { return _pos; }

	void setColor(const ImColor& color) { _color = color; }
	const ImColor& color() const { return _color; }

private:
	bool _active = false;
	ImVec2 _pos;
	ImColor _color {255, 255, 255, 255};
};

}