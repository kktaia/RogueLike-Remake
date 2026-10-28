#pragma once
#include <array>
#include <vector>
#include <optional>
#include <map>
#include "Vector2.h"
#include "ImgData.h"

struct Room {
	int x, y, w, h;

	int size() const { return w * h; }
};

struct Area
{
	int x, y, w, h;
	
	std::optional<Room> room;
};

class Field {
public:
	static constexpr int F_WIDTH = 48;
	static constexpr int F_HEIGHT = 32;
	static constexpr int MINI_PIXEL = 7;

	std::array<std::array<ImageID, F_WIDTH>, F_HEIGHT> field;
	Vector2 stairsPos;

	std::array<std::array<ImageID, F_WIDTH>, F_HEIGHT> covered;

	Field();

	void generateField();

	Vector2 getRandRoomPosition() const;

	[[nodiscard]] ImageID getTile(Vector2 pos) const;
	void setTile(Vector2 pos, ImageID id);

	[[nodiscard]] ImageID getCovered(Vector2 pos) const;
	void setCovered(Vector2 pos, ImageID id);

	[[nodiscard]] std::optional<Room> isRoom(Vector2 pos) const;

	void updateMask(Vector2 pos);

	int miniMap(Vector2 pos) const;

private:
	std::vector<Area> areas;
	int miniMapScreen;
	int maskScreen;
	int renderScreen;

	void generateMap();

	void resetMask();

	bool isWall(Vector2 pos) const;
	int getWallAround(Vector2 pos) const;
};