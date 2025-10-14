#include "SceneGameOver.h"

#include<DxLib.h>

#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"

SceneGameOver::SceneGameOver(void)
{
}

void SceneGameOver::Init(void)
{
	count_ = 0;
}

void SceneGameOver::Update(void)
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

void SceneGameOver::Draw(void)
{
	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 80, Application::SCREEN_SIZE_Y / 2, 0xffffff, "ゲームオーバー");


	if (count_ >= 120) {
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 20, 0xffffff, "クリックでタイトルへ");
	}
}

void SceneGameOver::Release(void)
{
}

void SceneGameOver::DrawDebug(void)
{
}
