#pragma once
#include "Chara.h"

class Player : public Character {
public:
	int hunger, hungerMax;
	
	int money;
	
	int weapon, armor;

	bool hasCheckStairs;

	Player();

	bool act(Field& field) override;

	void onTurnProcess() override;

	void deltaHunger(int delta);
};