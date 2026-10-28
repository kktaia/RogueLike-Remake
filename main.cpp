////////////////////////////////////////////////
//
//  j1806 柿田浩佑
//  
//  参考文献
//  https://youtu.be/xEA-K5HSw08
//  
////////////////////////////////////////////////

#include "def.h"
#include "Control.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("柿田の春休み");
	ChangeWindowMode(TRUE);
	SetGraphMode(SCREEN_W, SCREEN_H, 32);

	if (DxLib_Init() == -1) { return -1; }			// ＤＸライブラリ初期化処理

	Control control;

	while (ProcessMessage() == 0)
	{
		control.update();
		ScreenFlip();
	}

	
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了
}