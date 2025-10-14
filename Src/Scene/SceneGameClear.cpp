#include "SceneGameClear.h"

#include<DxLib.h>

#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"
#include "../Application.h"

#include <cmath>
#include <cstdlib>

SceneGameClear::SceneGameClear(void)
{
}

void SceneGameClear::Init(void)
{
	count_ = 0;
}

void SceneGameClear::Update(void)
{
	
	count_++;
	//シーン遷移
	if (
		
	InputManager::GetInstance().IsTrgMouseLeft())
	{

		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);

		//処理終了
		return;
	}
}

void SceneGameClear::Draw(void)
{

	
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(255, 255, 255), TRUE);

	DrawFormatString(Application::SCREEN_SIZE_X/2-80, Application::SCREEN_SIZE_Y / 2, 0x000000, "ゲームクリア");

	
	if (count_ >= 120) {
		DrawFormatString(0, Application::SCREEN_SIZE_Y-20, 0x000000, "クリックでタイトルへ");
	}
}

void SceneGameClear::Release(void)
{
}

void SceneGameClear::DrawDebug(void)
{
}
