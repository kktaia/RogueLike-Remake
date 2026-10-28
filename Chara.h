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
			Vector2 targetPos = pos * PIXEL;
			int speed = 2;

			Vector2 diff = targetPos - pixelPos;
			if (diff.abs().x <= speed && diff.abs().y <= speed) {
				pixelPos = targetPos;
			}
			else {
				pixelPos += diff.sign() * speed;
			}

			if (pixelPos != targetPos) {
				currentAnim = AnimState::WALK;
				animator.update();

				return false; // 移動中は入力を受け付けない
			}

			isMoving = false;
		}

		Vector2 nextPos = pos;

		//入力に応じて移動先を決定
		nextPos += Vector2::WAY4()[(int)dest];
		way = dest;

		//移動先が床なら移動
		if (field.getTile(nextPos) == ImageID::FLOOR) {
			pos = nextPos;
			isMoving = true;
			currentAnim = AnimState::WALK;
			animator.update();

			return true;
		}
		else {
			currentAnim = AnimState::IDLE;
			animator.reset();
		}

		return false;
	}

	int getCurrentImage() {
		return animator.getImage(way, currentAnim);
	}
};