#include "Camera.h"
#include "def.h"
#include <DxLib.h>
#include <cmath>

Vector2 Camera::pos = { 0, 0 };
int Camera::maskScreen = -1;

void Camera::update(Vector2 targetPixelPos) {
	//プレイヤーを画面の中心に配置するため、左上の座標を計算
	pos.x = targetPixelPos.x - (SCREEN_W - PIXEL) / 2;
	pos.y = targetPixelPos.y - (SCREEN_H - PIXEL) / 2;
}

void Camera::fieldDraw(Field& field) {
	//フィールドの描画
	int startX = (int)std::floor((float)pos.x / PIXEL) - 1;
	int startY = (int)std::floor((float)pos.y / PIXEL) - 1;
	int endX = (int)std::floor((float)(pos.x + SCREEN_W) / PIXEL) + 1;
	int endY = (int)std::floor((float)(pos.y + SCREEN_H) / PIXEL) + 1;

	for (int y = startY; y <= endY; y++) {
		for (int x = startX; x <= endX; x++) {
			Vector2 p = { x,y };
			int tileImg = ImgData::Get(ImageID::WALL);
			int coverImg = ImgData::Get(ImageID::NONE);

			if (x >= 0 && x < Field::F_WIDTH && y >= 0 && y < Field::F_HEIGHT) {
				tileImg = ImgData::Get(field.getTile(p));
				coverImg = ImgData::Get(field.getCovered(p));
			}

			Vector2 sp = p * PIXEL - pos;
			DrawExtendGraph(sp.x, sp.y, sp.x + PIXEL, sp.y + PIXEL, tileImg, TRUE);
			DrawExtendGraph(sp.x, sp.y, sp.x + PIXEL, sp.y + PIXEL, coverImg, TRUE);
		}
	}
}

void Camera::playerDraw(Player& player) {
	int playerImg = player.getCurrentImage();

	Vector2 sp = player.pixelPos - pos;
	DrawExtendGraph(sp.x, sp.y - 0.4 * PIXEL, sp.x + PIXEL, sp.y + PIXEL, playerImg, TRUE);
}

void Camera::darkMask(Field& field, Player& player) {
	SetDrawScreen(maskScreen);

	DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(150, 150, 150), TRUE);

	auto room = field.isRoom(player.pos);

	if (room.has_value()) {
		int x1 = (room->x - 0.5) * PIXEL - pos.x;
		int y1 = (room->y - 0.5) * PIXEL - pos.y;
		int x2 = (room->x + room->w + 0.5) * PIXEL - pos.x;
		int y2 = (room->y + room->h + 0.5) * PIXEL - pos.y;

		DrawRoundRect(x1, y1, x2, y2, 20, 20, WHITE, TRUE);
	}
	else {
		Vector2 p = player.pixelPos - pos + (Vector2(PIXEL / 2, PIXEL / 2));

		DrawCircle(p.x, p.y, PIXEL * 1.5, WHITE, TRUE);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
	DrawGraph(0, 0, maskScreen, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Camera::render(Field& field, Player& player) {
	if (maskScreen == -1) {
		maskScreen = MakeScreen(SCREEN_W, SCREEN_H, FALSE);
	}

	//フィールドの描画
	fieldDraw(field);

	//プレイヤーの描画
	playerDraw(player);

	//暗闇のマスク
	darkMask(field, player);
}