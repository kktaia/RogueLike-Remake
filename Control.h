#pragma once
#include "def.h"
#include "Field.h"
#include "Player.h"
#include "Menu.h"
#include "TurnManager.h"
#include <memory>

using std::unique_ptr;

class Control {
private:
	ActionMap actions;

	bool stairsMenuDismissed = false;

	void openStairsMenu();

public:
	unique_ptr<Field> field;
	unique_ptr<Player> player;
	TurnManager turnManager;

	int currentFloor;

	GameState state;
	Menu* currentMenu;

	Control();

	void actionInit();

	void update();
	void goNextFloor();
};