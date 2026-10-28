#pragma once
#include <map>

using std::map;

enum class ImageID {
	NONE,
	PLAYER,

	FLOOR,
	WALL,

	STAIRS,
};

class ImgData {
public:
	static void Load();
	static int Get(ImageID id);

private:
	static map<ImageID, int> images;
};