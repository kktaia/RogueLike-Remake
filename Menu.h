#pragma once
#include "Frame.h"
#include <functional>
#include <vector> 
#include <string>
#include <map>
#include <memory>

class Control;

struct MenuItem {
	std::string title;
	std::function<bool()> onClick;
};

class Menu : public FrameUI {
private:
	std::vector<MenuItem> items;
	int cursorIdx;

public:
	Menu() : cursorIdx(0) {}
	Menu(int _x, int _y, std::vector<MenuItem> _items);

	int maxStringLength() override;

	bool update() override;
	void draw() override;

	void reset();
};


enum class MenuID {
	MAIN,
	STAIRS,
};

enum class ActionID {
	NONE,
	OPEN_MESSAGE_HISTORY,
	GO_NEXT_FLOOR,
	CANCEL,
};

using ActionMap = std::map<ActionID, std::function<bool()>>;

class MenuManager {
public:
	static void menuInit(const ActionMap& actions);
	static Menu* Get(MenuID type);
	static void Reset(MenuID type);

private:
	static std::map<MenuID, std::unique_ptr<Menu>> menuData;

	static std::unique_ptr<Menu> makeMenu(int _x, int _y, std::vector<MenuItem> _items);
};