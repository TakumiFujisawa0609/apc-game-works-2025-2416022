#include "SceneGame.h"

#include<DxLib.h>

#include "../Object/Stage/Stage.h"
#include "../Object/Neko/Neko.h"
#include"../Common/Easing.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"

SceneGame::SceneGame(void)
{
	neko_ = nullptr;
	stage_ = nullptr;
}

void SceneGame::Init(void)
{
	// ステージ初期化
	stage_ = new Stage();
	stage_->Init();

	neko_ = new Neko();
	neko_->Init();
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

	// ステージ更新
	stage_->Update();

	neko_->Update();
}

void SceneGame::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "ゲームシーン");

	// ステージ描画
	stage_->Draw();

	neko_->Draw();

#ifdef _DEBUG
	//デバック表示
	DrawDebug();
#endif // _DEBUG
}

void SceneGame::Release(void)
{
	neko_->Release();
	delete neko_;

	// ステージ解放
	stage_->Release();
	delete stage_;
}

void SceneGame::DrawDebug(void)
{

}

