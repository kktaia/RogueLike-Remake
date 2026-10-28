#pragma once
#include "Vector2.h"
#include "Field.h"
#include "Player.h"

class Camera {
private:
	static Vector2 pos; // カメラの位置
	static int maskScreen;

	static void fieldDraw(Field& field);
	static void playerDraw(Player& player);

	static void darkMask(Field& field, Player& player);

public:
	//ターゲットに合わせてカメラの位置を更新する
	static void update(Vector2 targetPixelPos);

	static void render(Field& field, Player& player);
};