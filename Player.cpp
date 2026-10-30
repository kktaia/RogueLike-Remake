#include "Player.h"
#include "Input.h"
#include "Field.h"

Player::Player() {
	int gh[12];
	LoadDivGraph("Graphic/Player.png", 12, 3, 4, 20, 28, gh);

	hp = hpMax = 40;
	hunger = hungerMax = 100;
	atk = 10;
	def = 5;

	money = 0;

	weapon = 0;
	armor = 0;

	hasCheckStairs = false;

	animator = Animator();

	animator.animAdd(WAY::SOUTH, AnimState::IDLE, { gh[1] });
	animator.animAdd(WAY::SOUTH, AnimState::WALK, { gh[0], gh[1], gh[2], gh[1] });

	animator.animAdd(WAY::WEST, AnimState::IDLE, { gh[4] });
	animator.animAdd(WAY::WEST, AnimState::WALK, { gh[3], gh[4], gh[5], gh[4] });

	animator.animAdd(WAY::EAST, AnimState::IDLE, { gh[7] });
	animator.animAdd(WAY::EAST, AnimState::WALK, { gh[6], gh[7], gh[8], gh[7] });

	animator.animAdd(WAY::NORTH, AnimState::IDLE, { gh[10] });
	animator.animAdd(WAY::NORTH, AnimState::WALK, { gh[9], gh[10], gh[11], gh[10] });
}

bool Player::act(Field& field) {
	if (isMoving) {
		updateMovement();
		return false;
	}

	auto dest = Input::GetWay();

	if (dest.has_value()) {
		return move(field, dest.value());
	}
	
	currentAnim = AnimState::IDLE;
	animator.reset();

	return false;
}

void Player::onTurnProcess() {
	turnCount++;
	if (turnCount % 10 == 0) {
		deltaHunger(-1);
	}
}

void Player::deltaHunger(int delta) {
	hunger += delta;
	if (hunger > hungerMax) {
		hunger = hungerMax;
	}
	else if (hunger < 0) {
		hunger = 0;
	}
}