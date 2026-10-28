#include "Frame.h"
#include "UI.h"
#include "def.h"
#include <algorithm>
#include <DxLib.h>

int Frame::framesGh[9] = {};
int FrameUI::textFont = 0;

void Frame::drawWindowBox() {
	int x2 = x + w;
	int y2 = y + h;

	//４辺
	DrawExtendGraph(x + CS, y, x2 - CS, y + CS, framesGh[1], TRUE); // 上辺
	DrawExtendGraph(x + CS, y2 - CS, x2 - CS, y2, framesGh[7], TRUE); // 下辺
	DrawExtendGraph(x, y + CS, x + CS, y2 - CS, framesGh[3], TRUE); // 左辺
	DrawExtendGraph(x2 - CS, y + CS, x2, y2 - CS, framesGh[5], TRUE); // 右辺

	//中央を拡大して描画
	DrawExtendGraph(x + CS, y + CS, x2 - CS, y2 - CS, framesGh[4], TRUE);
	//４隅
	DrawGraph(x, y, framesGh[0], TRUE); // 左上
	DrawGraph(x2 - CS, y, framesGh[2], TRUE); // 右上
	DrawGraph(x, y2 - CS, framesGh[6], TRUE); // 左下
	DrawGraph(x2 - CS, y2 - CS, framesGh[8], TRUE); // 右下
}

FrameUI::FrameUI() {
	maxLines = 0;

	if(textFont == 0)textFont = CreateFontToHandle("ＭＳ ゴシック", 22, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 2);

	frame = Frame(0, 0, 0, 0);
}

void FrameUI::init(int _x, int _y, int _maxLines) {
	maxLines = _maxLines;

	int _w = maxStringLength() + 40;
	int _h = getLineHeight() * maxLines + 40; // 上下20pxずつの余白を追加
	frame = Frame(_x, _y, _w, _h);
}

int FrameUI::getLineHeight() const {
	static int height = -1;

	if (height == -1) {
		std::string dummyText = "あ";
		GetDrawStringSizeToHandle(nullptr, &height, nullptr, dummyText.c_str(), dummyText.length(), textFont); // フォントサイズを取得するためのダミー呼び出し
	}

	return height;
}

void FrameUI::drawText(int _x, int _y, int color, const std::string& text) {
	int clipX1 = frame.x + 20;
	int clipY1 = frame.y + 20;
	int clipX2 = frame.x + frame.w - 20;
	int clipY2 = frame.y + frame.h - 20;

	SetDrawArea(clipX1, clipY1, clipX2, clipY2);
	DrawFormatStringToHandle(clipX1 + _x, clipY1 + _y, color, textFont, text.c_str());
	SetDrawArea(0, 0, SCREEN_W, SCREEN_H); // 描画領域をリセット
}

int TextBox::maxStringLength() {
	int maxW = 0;
	for (const auto& line : text) {
		int w = GetDrawStringWidthToHandle(line.c_str(), line.length(), textFont);
		if (w > maxW) maxW = w;
	}
	// アイコン(＞)の分と少しの余白を足して返す
	return maxW;
}

void TextBox::draw() {
	frame.drawWindowBox();

	for(int i = 0; i < text.size(); i++) {
		drawText(0, i * getLineHeight(), WHITE, text[i]);
	}
}

int MessageWindow::maxStringLength() {
	std::string text = "あああああああああああああああああああああ";
	return 560;
	return GetDrawStringWidthToHandle(text.c_str(), text.length(), textFont);
}

bool MessageWindow::update() {
	if (deltaTime <= 0) {
		messages.clear(); // タイマーが切れたらメッセージをクリア
		return false;
	}

	if (isAnimating) {
		scrollY++;
		if (scrollY >= getLineHeight()) {
			scrollY = 0;
			messages.erase(messages.begin()); // 古いメッセージを削除して最大数を維持
			if (messages.size() <= MAX_LOG) {
				isAnimating = false; // アニメーション終了
			}
		}
	}
	else {
		deltaTime--;
	}

	return true;
}

void MessageWindow::draw() {
	frame.drawWindowBox();
	for (int i = 0; i < (std::min)((int)messages.size(), MAX_LOG); i++) {
		drawText(0, i * getLineHeight() - scrollY, WHITE, messages[i]);
	}
}

void MessageWindow::addMessage(std::string msg) {
	if (messages.size() >= MAX_LOG) {
		isAnimating = true; // アニメーション開始
	}
	messages.push_back(msg);
	
	deltaTime = 300; // メッセージ表示のタイマーをリセット（例: 300フレーム = 5秒）
}