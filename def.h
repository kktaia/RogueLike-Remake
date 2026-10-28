#pragma once
#include <DxLib.h>

const int RED = GetColor(255, 0, 0);
const int GREEN = GetColor(0, 255, 0);
const int BLUE = GetColor(0, 0, 255);
const int YELLOW = GetColor(255, 255, 0);
const int CYAN = GetColor(0, 255, 255);
const int MAGENTA = GetColor(255, 0, 255);
const int ORANGE = GetColor(255, 165, 0);
const int PURPLE = GetColor(128, 0, 128);
const int GRAY = GetColor(128, 128, 128);
const int PINK = GetColor(255, 192, 203);
const int BROWN = GetColor(165, 42, 42);
const int BEIGE = GetColor(245, 245, 220);
const int BLACK = GetColor(0, 0, 0);
const int WHITE = GetColor(255, 255, 255);

constexpr int SCREEN_W = 900;  //画面横幅
constexpr int SCREEN_H = 600;  //画面縦幅
constexpr int PIXEL = 60;      //ピクセル
constexpr int FIELD_W = 40;                  //マップの横幅
constexpr int FIELD_H = (FIELD_W / 2);      //マップの縦幅

enum class WAY{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	COUNT
};

enum class GameState {
	PLAY,
	MENU
};