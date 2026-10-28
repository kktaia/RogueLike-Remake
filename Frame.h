#pragma once
#include "UI.h"
#include <string>
#include <vector>

class Control;

class Frame {
public:
	static constexpr int CS = 6;
	static int framesGh[9];

private:
	int x, y, w, h;

	friend class FrameUI;

public:
	Frame(int _x = 0, int _y = 0, int _w = 0, int _h = 0) : x(_x), y(_y), w(_w), h(_h) {}
	~Frame() = default;

	void drawWindowBox();
};

class FrameUI :public UIElement {
protected:
	Frame frame;
	static int textFont;
	int maxLines;

public:
	FrameUI();

	void init(int _x, int _y, int _maxLines);

	int getLineHeight() const;
	void drawText(int _x, int _y, int color, const std::string& text);

	virtual int maxStringLength() = 0;

	virtual bool update() = 0;
	virtual void draw() = 0;
};

class TextBox :public FrameUI {
public:
	std::vector<std::string> text;

	TextBox(int x, int y, int maxLines, std::vector<std::string> _text) 
		: text(std::move(_text)) { init(x, y, maxLines); }

	int maxStringLength() override;

	bool update() override { return true; }
	void draw() override;
};

class MessageWindow :public FrameUI {
public:
	std::vector<std::string> messages;

	MessageWindow(int x, int y) { init(x, y, MAX_LOG); }

	int maxStringLength() override;

	bool update() override;
	void draw() override;

	void addMessage(std::string);

private:
	static const int MAX_LOG = 3; // ログに表示する最大メッセージ数
	int deltaTime = 0;
	bool isAnimating = false;
	int scrollY = 0;
};