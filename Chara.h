#pragma once
#include <string>
#include "Vector2.h"
#include "Animator.h"
#include "Field.h"
#include "TurnManager.h"

class Character :public TurnObserver {
public:
	Vector2 pos{}; // マップ上の座標
	Vector2 pixelPos{}; // ピクセル座標

	int hp = 0;
	int hpMax = 0;
	int atk = 0;
	int def = 0;

	WAY way = WAY::SOUTH;
	bool life = true;
	int turnCount = 0;
	bool isMoving = false;

	Animator animator{};
	AnimState currentAnim = AnimState::IDLE;

	Character() = default;
	virtual ~Character() = default;

	virtual void onTurnProcess() = 0;

	virtual bool act(Field& field) = 0;

	bool move(Field& field, WAY dest) {
		if (isMoving) {
			return false;
		}
		

		//入力に応じて移動先を決定
		way = dest;
		Vector2 nextPos = pos + Vector2::WAY4()[static_cast<int>(dest)];

		//移動先が床じゃなければ移動できない
		if (field.getTile(nextPos) != ImageID::FLOOR) {
			currentAnim = AnimState::IDLE;
			animator.reset();

			return false;
		}

		pos = nextPos;
		isMoving = true;
		currentAnim = AnimState::WALK;
		animator.update();

		return true;
	}

	void updateMovement() {
		if (!isMoving) {
			return;
		}
		
		Vector2 targetPos = pos * PIXEL;
		Vector2 diff = targetPos - pixelPos;
		const int speed = 2;

		if (diff.abs().x <= speed && diff.abs().y <= speed) {
			pixelPos = targetPos;
			isMoving = false;
			currentAnim = AnimState::IDLE;
			animator.reset();
		}
		else {
			pixelPos += diff.sign() * speed;
			animator.update();
		}
	}

	int getCurrentImage() {
		return animator.getImage(way, currentAnim);
	}
};