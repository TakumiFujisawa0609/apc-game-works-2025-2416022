#include "SceneGame.h"

#include <DxLib.h>

#include "../Object/Stage/Stage.h"
#include "../Object/Neko/Neko.h"
#include "../Object/Item/Food.h"
#include "../Object/Item/Wall.h"
#include "../Object/Message.h"
#include "../Common/Easing.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"

SceneGame::SceneGame(void)
{
	neko_ = nullptr;
	stage_ = nullptr;
	food_ = nullptr;
	wall_ = nullptr;
	message_ = nullptr;
	isEnd_ = false;
}

void SceneGame::Init(void)
{
	stage_ = new Stage();
	stage_->Init();

	neko_ = new Neko();
	neko_->Init();

	food_ = new Food();
	food_->Init();

	wall_ = new Wall();
	wall_->Init();

	message_ = new Message();
	message_->Init();

	count_ = 0;
	isEnd_ = false;
}

void SceneGame::Update(void)
{
	// ★ シーン終了中は何もしない
	if (isEnd_) return;

	auto& input = InputManager::GetInstance();

	// 各オブジェクトの更新
	stage_->Update();
	neko_->Update();
	food_->Update();
	if (food_->IsGameOver())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
		isEnd_ = true;
		return;
	}
	wall_->Update();
	if (wall_->IsGameOver())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
		isEnd_ = true;
		return;
	}
	message_->Update();

	// ゲーム進行カウント
	count_++;
	if (count_ >= 3200)
	{
		isEnd_ = true;
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
		return;
	}
}

void SceneGame::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "ゲームシーンです。異常を発見したらクリックで対処してください。");

	stage_->Draw();
	neko_->Draw();
	food_->Draw();
	wall_->Draw();
	message_->Draw();

	if (wall_->GetFlagImg() && wall_->GetIsMouseOver())
	{
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 40, 0xffffff, "壁に穴が開いている。異常だ。");
	}
	else if (wall_->GetIsMouseOver())
	{
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 40, 0xffffff, "ただの壁だ。");
	}

	if (food_->GetFlag() && food_->GetIsMouseOver())
	{
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 40, 0xffffff, "餌が無くなっている。補充してあげよう。");
	}
	else if (food_->GetIsMouseOver())
	{
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 40, 0xffffff, "餌はまだ残っている。");
	}

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG
}

void SceneGame::Release(void)
{
	// ★ 二重解放防止
	if (isEnd_) return;
	isEnd_ = true;

	if (neko_)
	{
		neko_->Release();
		delete neko_;
		neko_ = nullptr;
	}

	if (stage_)
	{
		stage_->Release();
		delete stage_;
		stage_ = nullptr;
	}

	if (food_)
	{
		food_->Release();
		delete food_;
		food_ = nullptr;
	}

	if (wall_)
	{
		wall_->Release();
		delete wall_;
		wall_ = nullptr;
	}

	if (message_)
	{
		message_->Release();
		delete message_;
		message_ = nullptr;
	}
}

void SceneGame::DrawDebug(void)
{
}
