#pragma once
#include "Player.h"
#include <memory>
#include <string>

class MessageWindow;

void DrawEdgeFormatString(int x, int y, int color, int edgeColor, const char* format, ...);

class UIElement {
public:
	virtual ~UIElement() = default;
	virtual bool update() = 0;
	virtual void draw() = 0;
};

class UI {
private:
	static std::unique_ptr<MessageWindow> msgWindow;

public:
	static int fontHeader;
	static int fontBody;

	//UIの初期化（フォントの読み込みなど）
	static void init();

	static void drawMessageWindow();

	static void drawTopBar(const Player& player, int floor);

	static void addMessage(const std::string& message);

	//全画面レイアウトで表示
	static void draw(const Player& player, int floor);
};