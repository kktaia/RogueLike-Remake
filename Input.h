#pragma once
#include <optional>
#include "Vector2.h"
#include "def.h"

class Input {
private:
	static char keys[256];
	static char oldKeys[256];

public:
	static void update();

	static bool GetKey(int key);

	static bool GetKeyDown(int key);

	[[nodiscard]] static std::optional<WAY> GetWay();

	[[nodiscard]] static Vector2 GetVector();
};