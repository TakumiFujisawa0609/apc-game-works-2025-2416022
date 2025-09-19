#include "SceneGame.h"

#include<DxLib.h>

#include"../Common/Easing.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"

SceneGame::SceneGame(void)
{
}

void SceneGame::Init(void)
{


}

void SceneGame::Update(void)
{
	auto& input = InputManager::GetInstance();

	////シーン遷移(デバッグ)
	//if (input.IsTrgDown(KEY_INPUT_SPACE))
	//{
	//	//決定音
	//	sound.Play(SoundManager::SOUND::SE_PUSH);

	//	//BGM停止
	//	sound.Stop(SoundManager::SOUND::BGM_TITLE);

	//	//シーン遷移
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);

	//	//処理終了
	//	return;
	//}
}

void SceneGame::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "ゲームシーン");

#ifdef _DEBUG
	//デバック表示
	DrawDebug();
#endif // _DEBUG
}

void SceneGame::Release(void)
{

}

void SceneGame::DrawDebug(void)
{

}

