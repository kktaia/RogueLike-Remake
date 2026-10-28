#include "Field.h"
#include "ImgData.h"
#include "def.h"
#include <bitset>
#include <DxLib.h>

constexpr int ROOM_SIZE_MIN = 6; //部屋の最小サイズ
constexpr int AREA_SIZE_MIN = (ROOM_SIZE_MIN + 4); //領域の最小サイズ
constexpr int ROOM_NUM_MIN = 6; //部屋の最小数

Field::Field()
{
	int screenW = F_WIDTH * MINI_PIXEL;
	int scrrenH = F_HEIGHT * MINI_PIXEL;

	miniMapScreen = MakeScreen(screenW, scrrenH, TRUE);
	maskScreen = MakeScreen(screenW, scrrenH, TRUE);
	renderScreen = MakeScreen(screenW, scrrenH, TRUE);

	generateField();
}

void Field::generateField()
{
	areas.clear();

	for (auto& row : field)
	{
		row.fill(ImageID::WALL);
	}
	for (auto& row : covered) {
		row.fill(ImageID::NONE);
	}

	areas.push_back(Area{ 0, 0, F_WIDTH, F_HEIGHT, nullopt });

	auto splitArea = [&](auto& self,size_t index,int depth) {
		if (depth == 0)return;

		Area oldArea = areas[index];
		Area newArea{};

		int x, y, w, h;

		bool splitWay = GetRand(1) == 0;
		if (oldArea.w > oldArea.h * 1.5)
			splitWay = true;
		else if (oldArea.h > oldArea.w * 1.5)
			splitWay = false;

		if (splitWay)
		{
			//横に分割
			if (oldArea.w < AREA_SIZE_MIN * 2)return;
			
			//分割できる最大の右端
			int maxSplitW = oldArea.w - AREA_SIZE_MIN;

			//分割位置をランダムに決定
			oldArea.w = GetRand(maxSplitW - AREA_SIZE_MIN) + AREA_SIZE_MIN;

			x = oldArea.x + oldArea.w;
			y = oldArea.y;
			w = areas[index].w - oldArea.w;
			h = oldArea.h;
		}
		else
		{
			//縦に分割
			if (oldArea.h < AREA_SIZE_MIN * 2)return;

			//分割できる最大の下端
			int maxSplitH = oldArea.h - AREA_SIZE_MIN;

			//分割位置をランダムに決定
			oldArea.h = GetRand(maxSplitH - AREA_SIZE_MIN) + AREA_SIZE_MIN;

			x = oldArea.x;
			y = oldArea.y + oldArea.h;
			w = oldArea.w;
			h = areas[index].h - oldArea.h;
		}
		newArea = { x,y,w,h,nullopt };

		areas[index] = oldArea;
		areas.push_back(newArea);

		size_t newIndex = areas.size() - 1;

		self(self, index, depth - 1);
		self(self, newIndex, depth - 1);
	};

	//最大5階層まで分割
	splitArea(splitArea, 0, 5);

	int roomNeeded = ROOM_NUM_MIN;
	int areasLeft = areas.size();

	/* 部屋と通路の生成 */
	for (auto& area : areas) 
	{
		/* 部屋を作るかどうか */
		if (!(areasLeft > roomNeeded && GetRand(100) < 40))
		{
			/* roomの生成 */
			roomNeeded--;
	
			//部屋の最大サイズ
			int maxW = area.w - 4;
			int maxH = area.h - 4;
	
			//部屋のサイズをランダムに決定
			int rw = GetRand(maxW - ROOM_SIZE_MIN) + ROOM_SIZE_MIN;
			int rh = GetRand(maxH - ROOM_SIZE_MIN) + ROOM_SIZE_MIN;
	
			//部屋の位置をランダムに決定
			int rx = GetRand(maxW - rw) + area.x + 2;
			int ry = GetRand(maxH - rh) + area.y + 2;
	
			area.room = Room{ rx,ry,rw,rh };
			const Room& r = *area.room;
	
			//1. 部屋の部分を床にする
			for (int y = r.y; y < r.y + r.h; y++) 
			{
				for (int x = r.x; x < r.x + r.w; x++)
				{
					field[y][x] = ImageID::FLOOR;
				}
			}
	
			//2. 部屋から境界線へ十字の道を作る
	
			//上
			int pathTopX = r.x + GetRand(r.w - 1);
			for (int y = area.y; y < r.y; y++)
				field[y][pathTopX] = ImageID::FLOOR;
	
			//下
			int pathBottomX = r.x + GetRand(r.w - 1);
			for (int y = r.y + r.h; y < area.y + area.h; y++)
				field[y][pathBottomX] = ImageID::FLOOR;
	
			//左
			int pathLeftY = r.y + GetRand(r.h - 1);
			for (int x = area.x; x < r.x; x++)
				field[pathLeftY][x] = ImageID::FLOOR;
	
			//右
			int pathRightY = r.y + GetRand(r.h - 1);
			for (int x = r.x + r.w; x < area.x + area.w; x++)
				field[pathRightY][x] = ImageID::FLOOR;
		}
		else
		{
			/* 部屋を作らない場合は通路を作る */
			bool isPath[2] = { true,true };
			isPath[GetRand(1)] = GetRand(100) < 40;
	
			if (isPath[0]) {
				int crossX = area.x + 2 + GetRand(area.w - 5);
				for (int y = area.y; y < area.y + area.h; y++)
				{
					field[y][crossX] = ImageID::FLOOR;
				}
			}
	
			if (isPath[1]) {
				int crossY = area.y + 2 + GetRand(area.h - 5);
				for (int x = area.x; x < area.x + area.w; x++)
				{
					field[crossY][x] = ImageID::FLOOR;
				}
			}
		}
		
		//領域の境界線（下端と右端）を床にする
		//下端
		for (int x = area.x + 1; x < area.x + area.w - 1; x++)
		{
			if (area.y + area.h == F_HEIGHT)break; //フィールドの外壁を床にしない
	
			field[area.y + area.h - 1][x] = ImageID::FLOOR;
		}
		//右端
		for (int y = area.y + 1; y < area.y + area.h - 1; y++)
		{
			if (area.x + area.w == F_WIDTH)break; //フィールドの外壁を床にしない
	
			field[y][area.x + area.w - 1] = ImageID::FLOOR;
		}
	
		areasLeft--;
	}

	/* 行き止まりを削除 */

	//行き止まりを検知するラムダ関数
	auto isDeadEnd = [&](Vector2 pos) {
		if (getTile(pos) == ImageID::WALL) return false;

		int around = getWallAround(pos);

		return std::bitset<4>(around).count() >= 3; //周囲に壁が3つ以上ある場合は行き止まり
	};

	for (int y = 0; y < F_HEIGHT; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			Vector2 currentPos(x, y);

			while (isDeadEnd(currentPos)) {
				setTile(currentPos, ImageID::WALL);

				//次の行き止まりを探す
				for(auto& d : Vector2::WAY4()) {
					Vector2 nextPos = currentPos + d;

					if (!isWall(nextPos)) {
						currentPos = nextPos;
						break;
					}
				}
			}
		}
	}

	//階段の配置
	stairsPos = getRandRoomPosition();
	setCovered(stairsPos, ImageID::STAIRS);

	generateMap();
}

void Field::generateMap() {
	SetDrawScreen(miniMapScreen);
	ClearDrawScreen();

	for (int y = 0; y < F_HEIGHT; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			if (getTile(Vector2(x, y)) == ImageID::WALL)continue;
			int x1 = x * MINI_PIXEL;
			int y1 = y * MINI_PIXEL;
			int x2 = (x + 1) * MINI_PIXEL;
			int y2 = (y + 1) * MINI_PIXEL;

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			DrawBox(x1, y1, x2, y2, BLUE, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			int bitMask = getWallAround(Vector2(x, y));

			if (bitMask & 1)DrawLine(x1, y1, x2, y1, WHITE);
			if (bitMask & 2)DrawLine(x2 - 1, y1, x2 - 1, y2, WHITE);
			if (bitMask & 4)DrawLine(x1, y2 - 1, x2, y2 - 1, WHITE);
			if (bitMask & 8)DrawLine(x1, y1, x1, y2, WHITE);
		}
	}

	int sx = stairsPos.x * MINI_PIXEL;
	int sy = stairsPos.y * MINI_PIXEL;
	DrawBox(sx, sy, sx + MINI_PIXEL, sy + MINI_PIXEL, CYAN, FALSE);

	SetDrawScreen(DX_SCREEN_BACK);

	resetMask();
}

Vector2 Field::getRandRoomPosition() const
{
	if (areas.empty())return Vector2::Zero();

	//部屋を持つ領域からランダムに選ぶ
	std::vector<Area> roomAreas;
	for (const auto& area : areas) {
		if (area.room.has_value()) {
			roomAreas.push_back(area);
		}
	}

	int index = GetRand(roomAreas.size() - 1);
	const Room& r = *roomAreas[index].room;

	int x = r.x + GetRand(r.w - 1);
	int y = r.y + GetRand(r.h - 1);

	return Vector2(x, y);
}

ImageID Field::getTile(Vector2 pos) const
{
	if (pos.x < 0 || pos.x >= F_WIDTH || pos.y < 0 || pos.y >= F_HEIGHT)
		return ImageID::WALL;
	return field[pos.y][pos.x];
}

void Field::setTile(Vector2 pos, ImageID id)
{
	if (pos.x < 0 || pos.x >= F_WIDTH || pos.y < 0 || pos.y >= F_HEIGHT)
		return;
	field[pos.y][pos.x] = id;
}

ImageID Field::getCovered(Vector2 pos) const {
	if (pos.x < 0 || pos.x >= F_WIDTH || pos.y < 0 || pos.y >= F_HEIGHT)
		return ImageID::WALL;
	return covered[pos.y][pos.x];
}

void Field::setCovered(Vector2 pos, ImageID id) {
	if (pos.x < 0 || pos.x >= F_WIDTH || pos.y < 0 || pos.y >= F_HEIGHT)
		return;
	covered[pos.y][pos.x] = id;
}

std::optional<Room> Field::isRoom(Vector2 pos) const
{
	if (getTile(pos) != ImageID::FLOOR)return std::nullopt;
	for (const auto& area : areas) {
		if (area.room.has_value()) {
			const Room& r = *area.room;
			if (pos.x >= r.x && pos.x < r.x + r.w && pos.y >= r.y && pos.y < r.y + r.h) {
				return r;
			}
		}
	}
	return std::nullopt;
}

bool Field::isWall(Vector2 pos) const
{
	return getTile(pos) == ImageID::WALL;
}

int Field::getWallAround(Vector2 pos) const
{
	int bitmask = 0;

	for (int i = 0; i < 4; i++) {
		if (isWall(pos + Vector2::WAY4()[i]))bitmask |= (1 << i);
	}

	return bitmask;
}

void Field::resetMask() {
	SetDrawScreen(maskScreen);
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawBox(0, 0, F_WIDTH * MINI_PIXEL, F_HEIGHT * MINI_PIXEL, WHITE, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawScreen(DX_SCREEN_BACK);
}

void Field::updateMask(Vector2 pos) {
	SetDrawScreen(maskScreen);
	auto room = isRoom(pos);
	int x1, y1, x2, y2;

	if (room.has_value()) {
		x1 = (room->x - 1) * MINI_PIXEL;
		y1 = (room->y - 1) * MINI_PIXEL;
		x2 = x1 + (room->w + 2) * MINI_PIXEL;
		y2 = y1 + (room->h + 2) * MINI_PIXEL;
		
	}
	else {
		x1 = (pos.x - 1) * MINI_PIXEL;
		y1 = (pos.y - 1) * MINI_PIXEL;
		x2 = (pos.x + 2) * MINI_PIXEL;
		y2 = (pos.y + 2) * MINI_PIXEL;
	}
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawBox(x1, y1, x2, y2, WHITE, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(miniMapScreen);

	//DrawCircle((pos.x + 0.5) * MINI_PIXEL, (pos.y + 0.5) * MINI_PIXEL, MINI_PIXEL / 2, WHITE, TRUE);

	SetDrawScreen(DX_SCREEN_BACK);
}

int Field::miniMap(Vector2 pos) const{
	SetDrawScreen(renderScreen);
	ClearDrawScreen();

	DrawGraph(0, 0, miniMapScreen, TRUE);

	for (int y = 0; y < F_HEIGHT; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			if (getCovered(Vector2(x, y)) == ImageID::NONE)continue;
			int x0 = (x + 0.5) * MINI_PIXEL;
			int y0 = (y + 0.5) * MINI_PIXEL;
			int r = MINI_PIXEL / 2;
			
			switch (getCovered(Vector2(x, y))) 
			{
			default:
				break;
			}
		}
	}

	float x = (pos.x + 0.5) * MINI_PIXEL;
	float y = (pos.y + 0.5) * MINI_PIXEL;
	float r = (MINI_PIXEL / 2.0) * 0.8;

	DrawCircleAA(x, y, r, 32, WHITE, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_MUL_A, 255);
	//DrawGraph(0, 0, maskScreen, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);

	return renderScreen;
}