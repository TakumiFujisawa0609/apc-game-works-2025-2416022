#include "SceneGame.h"

#include<DxLib.h>

#include "../Object/Stage/Stage.h"
#include "../Object/Neko/Neko.h"
#include "../Object/Item/Food.h"
#include "../Object/Item/Wall.h"
#include"../Common/Easing.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"

SceneGame::SceneGame(void)
{
	neko_ = nullptr;
	stage_ = nullptr;
	food_ = nullptr;
	wall_ = nullptr;
}

void SceneGame::Init(void)
{
	// ステージ初期化
	stage_ = new Stage();
	stage_->Init();

	neko_ = new Neko();
	neko_->Init();

	food_ = new Food();
	food_->Init();

	wall_ = new Wall();
	wall_->Init();

	count_ = 0;
}

void SceneGame::Update(void)
{
	auto& input = InputManager::GetInstance();

	// ステージ更新
	stage_->Update();

	neko_->Update();

	food_->Update();

	wall_->Update();

	count_++;
	if (count_ >= 3200)
	{
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);

		//処理終了
		return;
	}
}

void SceneGame::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "ゲームシーンです。異常を発見したらクリックで対処してください。");

	// ステージ描画
	stage_->Draw();

	neko_->Draw();

	food_->Draw();

	wall_->Draw();

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

	food_->Release();
	delete food_;

	wall_->Release();
	delete wall_;
}

void SceneGame::DrawDebug(void)
{

}

