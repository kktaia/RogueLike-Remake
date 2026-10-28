#include "Menu.h"
#include "Input.h"

Menu::Menu(int _x, int _y, std::vector<MenuItem> _items) 
	:items(std::move(_items)),cursorIdx(0){

	init(_x, _y, static_cast<int>(items.size()));
}

int Menu::maxStringLength() {
	int maxW = 0;
	for (const auto& item : items) {
		int w = GetDrawStringWidthToHandle(item.title.c_str(), item.title.length(), textFont);
		if (w > maxW) maxW = w;
	}
	// アイコン(＞)の分と少しの余白を足して返す
	return maxW + (getLineHeight() * 2);
}

bool Menu::update() {
	if (Input::GetKeyDown(KEY_INPUT_UP)) {
		cursorIdx--;
	}
	else if (Input::GetKeyDown(KEY_INPUT_DOWN)) {
		cursorIdx++;
	}
	else if (Input::GetKeyDown(KEY_INPUT_RETURN)) {
		return items[cursorIdx].onClick();
	}

	bool isLoop = false;
	if (isLoop) {
		// 上下ループ
		if (cursorIdx < 0) {
			cursorIdx += (int)items.size();
		}
		else if (cursorIdx >= items.size()) {
			cursorIdx -= (int)items.size();
		}

	}
	else {
		// 上下で止まる
		if (cursorIdx < 0) {
			cursorIdx = 0;
		}
		else if (cursorIdx >= (int)items.size()) {
			cursorIdx = (int)items.size() - 1;
		}
	}

	return true; // メニューを開いたままにする
}

void Menu::draw() {
	frame.drawWindowBox();
	int lineHeight = getLineHeight();
	int x = lineHeight * 1.2;

	drawText(0, lineHeight * cursorIdx, YELLOW, "＞");
	for (int i = 0; i < items.size(); i++) {
		int color = (i == cursorIdx) ? YELLOW : WHITE;
		drawText(x, i * lineHeight, color, items[i].title);
	}
}

void Menu::reset() {
	cursorIdx = 0;
}


std::map<MenuID, std::unique_ptr<Menu>> MenuManager::menuData;

void MenuManager::menuInit(const ActionMap& actions) {
	menuData[MenuID::MAIN] = makeMenu(100, 100, {
		{"メッセージ履歴",actions.at(ActionID::OPEN_MESSAGE_HISTORY)},
		{"戻る",          actions.at(ActionID::CANCEL)}
	});

	menuData[MenuID::STAIRS] = makeMenu(350, 100, {
		{"進む", actions.at(ActionID::GO_NEXT_FLOOR)},
		{"戻る", actions.at(ActionID::CANCEL)}
	});
}

Menu* MenuManager::Get(MenuID type) {
	if (menuData.count(type) == 0) {
		return nullptr; // メニューが定義されていない場合はnullptrを返す
	}

	menuData[type]->reset();

	return menuData[type].get(); // メニューのポインタを返す
}

void MenuManager::Reset(MenuID type) {
	if (menuData.count(type) == 0) {
		return; // メニューが定義されていない場合は何もしない
	}
	menuData[type]->reset();
}

std::unique_ptr<Menu> MenuManager::makeMenu(int _x, int _y, std::vector<MenuItem> _items) {
	return std::make_unique<Menu>(_x, _y, std::move(_items));
}