#include "SceneGameOver.h"

#include<DxLib.h>

#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"

SceneGameOver::SceneGameOver(void)
{
}

void SceneGameOver::Init(void)
{

}

void SceneGameOver::Update(void)
{

	//シーン遷移(デバッグ)
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{

		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);

		//処理終了
		return;
	}
}

void SceneGameOver::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "ゲームオーバー");
}

void SceneGameOver::Release(void)
{
}

void SceneGameOver::DrawDebug(void)
{
}
