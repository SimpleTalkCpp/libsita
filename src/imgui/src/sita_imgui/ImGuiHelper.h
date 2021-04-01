#pragma once

inline void operator+=(ImVec2& a, const ImVec2& b) { a.x += b.x; a.y += b.y; }
inline void operator-=(ImVec2& a, const ImVec2& b) { a.x -= b.x; a.y -= b.y; }
inline void operator*=(ImVec2& a, const ImVec2& b) { a.x *= b.x; a.y *= b.y; }
inline void operator/=(ImVec2& a, const ImVec2& b) { a.x /= b.x; a.y /= b.y; }

inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x + b.x, a.y + b.y); }
inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x - b.x, a.y - b.y); }
inline ImVec2 operator*(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x * b.x, a.y * b.y); }
inline ImVec2 operator/(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x / b.x, a.y / b.y); }

inline ImVec2 operator+(const ImVec2& a, float b) { return ImVec2(a.x + b, a.y + b); }
inline ImVec2 operator-(const ImVec2& a, float b) { return ImVec2(a.x - b, a.y - b); }
inline ImVec2 operator*(const ImVec2& a, float b) { return ImVec2(a.x * b, a.y * b); }
inline ImVec2 operator/(const ImVec2& a, float b) { return ImVec2(a.x / b, a.y / b); }


