#pragma once
#include <array>
#include <cstddef>
#include <utility>
#include <vector>
#include "def.h"

enum class AnimState {
	IDLE,
	WALK,
	ATTACK,
	DAMAGE,
	DEATH,
	COUNT
};

class Animator {
private:
	using Frames = std::vector<int>; // フレームのリスト
	using AnimationTable = std::array<std::array<Frames, 
		static_cast<std::size_t>(AnimState::COUNT)>, 
		static_cast<std::size_t>(WAY::COUNT)>;

	int animCount = 0;
	int speed;

	// [WAY][AnimState] -> フレームのリスト
	AnimationTable animFrames{};

	// インデックスを取得するためのヘルパー関数
	static constexpr std::size_t index(WAY value) {
		return static_cast<std::size_t>(value);
	}
	static constexpr std::size_t index(AnimState value) {
		return static_cast<std::size_t>(value);
	}

public:
	Animator(int animSpeed = 24) : speed(animSpeed) {}

	void animAdd(WAY way, AnimState state, Frames imageList) {
		animFrames[index(way)][index(state)] = std::move(imageList);
	}

	void update() {
		animCount++;
	}

	void reset() {
		animCount = 0;
	}

	[[nodiscard]] int getImage(WAY way, AnimState state) const {
		const auto& animList = animFrames[index(way)][index(state)];

		if (animList.empty()) return -1; // フレームがない場合は-1を返す
		
		const auto frameIndex = static_cast<std::size_t>(animCount / speed) % animList.size();

		return animList[frameIndex];
	}
};