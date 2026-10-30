#include "Control.h"
#include "Field.h"
#include "ImgData.h"
#include "Input.h"
#include "Camera.h"
#include "UI.h"
#include "Menu.h"
#include <DxLib.h>
#include <time.h>
#include <Windows.h>
#include <map>
#include <functional>

Control::Control()
{
	SRand((unsigned int)time(NULL));
	if (AddFontResourceEx("Graphic/WorkSans-Bold.ttf", FR_PRIVATE, NULL) > 0) {
		// 読み込み成功（特にログなどを出さなくてもOK）
	}
	actionInit();
	MenuManager::menuInit(actions);
	ImgData::Load();
	UI::init();

	LoadDivGraph("Graphic/WindowBase.png", 9, 3, 3, Frame::CS, Frame::CS, Frame::framesGh);

	currentFloor = 1;
	state = GameState::PLAY;
	currentMenu = nullptr;

	field = std::make_unique<Field>();
	player = std::make_unique<Player>();

	turnManager.addObserver(*player);

	do
	{
		player->pos = field->getRandRoomPosition();
	} while (player->pos == field->stairsPos);

	player->pixelPos = player->pos * PIXEL;
}

void Control::actionInit()
{
	actions[ActionID::OPEN_MESSAGE_HISTORY] = [this]() {
		// メッセージ履歴を開くアクションの実装
		return false; // メニューを閉じる
	};

	actions[ActionID::GO_NEXT_FLOOR] = [this]() {
		goNextFloor();
		return false; // メニューを閉じる
	};

	actions[ActionID::CANCEL] = [this]() {
		state = GameState::PLAY;
		return false; // メニューを閉じる
	};
}

void Control::update()
{
	//入力の更新
	Input::update();

	//メニューの更新
	if (state == GameState::MENU && currentMenu != nullptr) {
		if (!currentMenu->update()) {
			

			currentMenu = nullptr; // メニューを閉じる
			state = GameState::PLAY; // プレイ状態に戻る
		}
	}
	else {
		if (player->life) {
			//player->move0(*field);
			if (player->act(*field)) {
				field->updateMask(player->pos);
				turnManager.notifyTurn();
			}
			
		}
		//shiftキーでメニューを開く
		if (Input::GetKeyDown(KEY_INPUT_LSHIFT) || Input::GetKeyDown(KEY_INPUT_RSHIFT)) {
			state = GameState::MENU;
			
			currentMenu = MenuManager::Get(MenuID::MAIN);
			return;
		}
		//階段メニューを開く
		if (player->pos != field->stairsPos) {
			player->hasCheckStairs = false;
		}
		if (!player->isMoving && player->pos == field->stairsPos && !player->hasCheckStairs) {
			state = GameState::MENU;

			currentMenu = MenuManager::Get(MenuID::STAIRS);
			player->hasCheckStairs = true;
			return;
		}
	}

	//描画
	Camera::update(player->pixelPos);
	Camera::render(*field, *player);
	UI::draw(*player, currentFloor);
	DrawGraph(50, 100, field->miniMap(player->pos), TRUE);

	if (currentMenu) {
		currentMenu->draw();
	}
}

void Control::goNextFloor()
{
	currentFloor++;
	field->generateField();
	player->hasCheckStairs = false;

	do
	{
		player->pos = field->getRandRoomPosition();
	} while (player->pos == field->stairsPos);

	player->pixelPos = player->pos * PIXEL;

	DrawBox(0, 0, SCREEN_W, SCREEN_H, BLACK, TRUE);
	ScreenFlip();
	SetFontSize(50);
	int textX = (SCREEN_W / 2) - 100;
	int textY = (SCREEN_H / 2) - 25;
	DrawFormatString(textX, textY, WHITE, "Ｂ%dＦ", currentFloor);
	ScreenFlip();

	WaitTimer(1500);

	UI::addMessage("Ｂ" + std::to_string(currentFloor) + "Ｆに降りた。");
}