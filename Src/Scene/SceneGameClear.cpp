#include "SceneGameClear.h"

#include<DxLib.h>

#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"

SceneGameClear::SceneGameClear(void)
{
}

void SceneGameClear::Init(void)
{

}

void SceneGameClear::Update(void)
{

	//シーン遷移(デバッグ)
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{

		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);

		//処理終了
		return;
	}
}

void SceneGameClear::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "ゲームクリア");
}

void SceneGameClear::Release(void)
{
}

void SceneGameClear::DrawDebug(void)
{
}
