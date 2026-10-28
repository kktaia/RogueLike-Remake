#include "UI.h"
#include "Frame.h"
#include "LogManager.h"
#include "def.h"
#include <DxLib.h>
#include <cstdarg>
#include <cstdio>

int UI::fontHeader = 0;
int UI::fontBody = 0;
std::unique_ptr<MessageWindow> UI::msgWindow = nullptr;

void UI::init() {
	fontHeader = CreateFontToHandle("Work Sans", 42, 8, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	fontBody = CreateFontToHandle("ＭＳ ゴシック", 18, 10, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
}

void DrawEdgeFormatStringToHandle(int x, int y, int color, int edgeColor, int fontHandle, const char* format, ...) {
	char buf[256];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);

	// 8方向（上下左右＋斜め）に黒で描画して、濃く隙間のない縁取りを作る
	DrawStringToHandle(x - 1, y - 1, buf, edgeColor, fontHandle);
	DrawStringToHandle(x, y - 1, buf, edgeColor, fontHandle);
	DrawStringToHandle(x + 1, y - 1, buf, edgeColor, fontHandle);
	DrawStringToHandle(x - 1, y, buf, edgeColor, fontHandle);
	DrawStringToHandle(x + 1, y, buf, edgeColor, fontHandle);
	DrawStringToHandle(x - 1, y + 1, buf, edgeColor, fontHandle);
	DrawStringToHandle(x, y + 1, buf, edgeColor, fontHandle);
	DrawStringToHandle(x + 1, y + 1, buf, edgeColor, fontHandle);

	// 最後に真ん中に本体色で描画
	DrawStringToHandle(x, y, buf, color, fontHandle);
}

void UI::drawMessageWindow() {
	if (!msgWindow) return;

	if (msgWindow->update()) {
		msgWindow->draw();
	}
	else{
		LogManager::addDelimiter(); // メッセージウィンドウが閉じるときに区切り線を追加
		msgWindow.reset();
	}
}

void UI::drawTopBar(const Player& player, int floor) {
	// ==========================================
	//  1. レイアウト管理（上部半透明バー）
	// ==========================================
	int topBarHeight = 90;
	int centerY = topBarHeight / 2;

	static int blur = MakeScreen(SCREEN_W, topBarHeight, FALSE);

	// ① カメラが描画した現在の画面（ダンジョンの背景）をコピーして切り取る
	GetDrawScreenGraph(0, 0, SCREEN_W, topBarHeight, blur);

	// ② 切り取った画像に直接ガウスブラー（ぼかし）をかける
	GraphFilter(blur, DX_GRAPH_FILTER_GAUSS, 64, 200);

	// ③ ぼかした画像を元の位置に貼り付ける
	DrawGraph(0, 0, blur, FALSE);

	// X座標の基準
	int floorX = 25, levelX = 140, labelX = 280, valueX = 370, barX = 480;


	// ==========================================
	//  2. 階層とレベル（欧文フォント "Work Sans" を適用）
	// ==========================================

	int bigTextY = centerY - (42 / 2);

	// %dF や Lv%d の描画
	DrawEdgeFormatStringToHandle(floorX, bigTextY, WHITE, BLACK, fontHeader, "B%dF", floor);
	DrawEdgeFormatStringToHandle(levelX, bigTextY, YELLOW, BLACK, fontHeader, "Lv%d", 1);


	// ==========================================
	//  3. HPと満腹度（日本語フォント "ＭＳ ゴシック" に戻す）
	// ==========================================
	int rowGap = 32;
	int hpY = centerY - rowGap + 5;
	int hungY = centerY + 10;
	int barH = 12;

	// --- HP ---
	DrawEdgeFormatStringToHandle(labelX, hpY, YELLOW, BLACK, fontBody, "ＨＰ");
	DrawEdgeFormatStringToHandle(valueX, hpY, WHITE, BLACK, fontBody, "%d/%d", player.hp, player.hpMax);

	int hpBarY = hpY + 4;
	//DrawBox(barX, hpBarY, barX + 160, hpBarY + barH, barBg, TRUE);
	int hpW = (int)((double)player.hp / player.hpMax * 160);
	if (hpW > 0) DrawBox(barX, hpBarY, barX + hpW, hpBarY + barH, GREEN, TRUE);
	DrawBox(barX, hpBarY, barX + 160, hpBarY + barH, BLACK, FALSE);

	// --- 満腹度 ---
	DrawEdgeFormatStringToHandle(labelX, hungY, CYAN, BLACK, fontBody, "満腹度");
	DrawEdgeFormatStringToHandle(valueX, hungY, WHITE, BLACK, fontBody, "%d/%d", player.hunger, player.hungerMax);

	int hungBarY = hungY + 4;
	//DrawBox(barX, hungBarY, barX + 160, hungBarY + barH, barBg, TRUE);
	int hungW = (int)((double)player.hunger / player.hungerMax * 160);
	if (hungW > 0) DrawBox(barX, hungBarY, barX + hungW, hungBarY + barH, CYAN, TRUE);
	DrawBox(barX, hungBarY, barX + 160, hungBarY + barH, BLACK, FALSE);

	// --- ターン数（右端・中央揃え） ---
	DrawEdgeFormatStringToHandle(SCREEN_W - 180, centerY - (42 / 2), YELLOW, BLACK, fontHeader, "%d G", player.money);
}

void UI::addMessage(const std::string& message) {
	if (msgWindow == nullptr) {
		int msgX = (SCREEN_W - 560) / 2;
		int msgY = SCREEN_H - 150;

		msgWindow = std::make_unique<MessageWindow>(msgX, msgY);
	}

	msgWindow->addMessage(message);
	LogManager::addLog(message);
}

void UI::draw(const Player& player, int floor)
{
	drawTopBar(player, floor);

	drawMessageWindow();

	
}