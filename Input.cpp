#include "Input.h"
#include <DxLib.h>

char Input::keys[256];
char Input::oldKeys[256];

void Input::update() {
	for (int i = 0; i < 256; i++) {
		oldKeys[i] = keys[i];
	}

	GetHitKeyStateAll(keys);
}

bool Input::GetKey(int key) {
	return keys[key] == 1;
}

bool Input::GetKeyDown(int key) {
	return keys[key] == 1 && oldKeys[key] == 0;
}

std::optional<WAY> Input::GetWay() {
	if (GetKey(KEY_INPUT_UP)) return WAY::NORTH;
	if (GetKey(KEY_INPUT_RIGHT))return WAY::EAST;
	if (GetKey(KEY_INPUT_DOWN))return WAY::SOUTH;
	if (GetKey(KEY_INPUT_LEFT))return WAY::WEST;

	return std::nullopt;
}

Vector2 Input::GetVector() {
	auto way = GetWay();
	
	if (way.has_value())
		return Vector2::WAY4()[(int)way.value()];
	
	return Vector2::Zero();
}