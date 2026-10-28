#pragma once
#include <cmath>
#include <array>

struct Vector2 {
public:
	int x = 0;
	int y = 0;

	constexpr Vector2() = default;
	constexpr Vector2(int _x, int _y) : x(_x), y(_y) {}

	[[nodiscard]] constexpr Vector2 operator+(const Vector2& other) const noexcept { return Vector2(x + other.x, y + other.y); }
	[[nodiscard]] constexpr Vector2 operator-(const Vector2& other) const noexcept { return Vector2(x - other.x, y - other.y); }

	Vector2 operator-() const { return Vector2(-x, -y); }

	Vector2 operator*(int scalar) const { return Vector2(x * scalar, y * scalar); }

	constexpr Vector2& operator+=(const Vector2& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}

	bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vector2& other) const { return !(*this == other); }

	Vector2 sign() const { return Vector2((x > 0) - (x < 0), (y > 0) - (y < 0)); }

	Vector2 abs() const { return Vector2(std::abs(x), std::abs(y)); }

	static Vector2 X() { return Vector2(1, 0); }
	static Vector2 Y() { return Vector2(0, 1); }
	static Vector2 Zero() { return Vector2(0, 0); }

	static std::array<Vector2, 4> WAY4() { return { -Y(), X(), Y(), -X() }; }
};