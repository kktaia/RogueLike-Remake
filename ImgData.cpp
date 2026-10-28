#include "ImgData.h"
#include <map>
#include <DxLib.h>

map<ImageID, int> ImgData::images;

void ImgData::Load()
{
	images[ImageID::NONE] = -1;

	images[ImageID::PLAYER] = LoadGraph("Graphic/player.png");

	images[ImageID::FLOOR] = LoadGraph("Graphic/floor.png");
	images[ImageID::WALL] = LoadGraph("Graphic/wall.png");

	images[ImageID::STAIRS] = LoadGraph("Graphic/stairs.png");
}

int ImgData::Get(ImageID id)
{
	return images[id];
}