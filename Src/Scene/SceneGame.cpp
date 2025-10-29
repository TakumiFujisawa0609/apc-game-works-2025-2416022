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

	neko_->SetFood(food_);

	img3_ = LoadGraph((Application::PATH_ITEM + "nc296608.png").c_str());

	fontHandle=CreateFontToHandle(NULL, 18, 9,-1);
}

void SceneGame::Update(void)
{
	if (isGameOver_)
	{
		// --- ゲームオーバー中 ---
		gameOverTimer_++;

		// 原因になったクラスだけ Update を続行
		switch (gameOverSource_)
		{
		case GameOverSource::FOOD:
			food_->Update();
			break;
		case GameOverSource::WALL:
			wall_->Update();
			break;
		default:
			break;
		}

		// --- 一定時間後に遷移 ---
		if (gameOverTimer_ >= GAMEOVER_WAIT_)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			isEnd_ = true;
			return;
		}

		return; // ゲームオーバー中は他の更新を行わない
	}

	// --- 通常時の更新 ---
	stage_->Update();
	neko_->Update();
	food_->Update();
	wall_->Update();
	message_->Update();

	// --- Food ゲームオーバー判定 ---
	if (food_->IsGameOver())
	{
		StartGameOver(GameOverSource::FOOD);
		return;
	}

	// --- Wall ゲームオーバー判定 ---
	if (wall_->IsGameOver())
	{
		StartGameOver(GameOverSource::WALL);
		return;
	}

	// --- ゲームクリアタイマー ---
	count_++;
	if (count_ >= 3200)
	{
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

	DrawInfo();

	if (isGameOver_)
	{
		if (gameOverSource_ == GameOverSource::FOOD)
		{
			DrawStringToHandle(0, Application::SCREEN_SIZE_Y - 40, "食べ過ぎには注意しましょう。ネコの管理もあなたの仕事です。", GetColor(255, 0, 0),fontHandle);
		}
		else if (gameOverSource_ == GameOverSource::WALL)
		{
			// --- ゲームオーバー画像 ---
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			1.0, 0.0, img3_, true);
		
			DrawStringToHandle(0, Application::SCREEN_SIZE_Y - 40, "壁の穴を放置してはいけません。修繕費用はあなた持ちです。", GetColor(255, 0, 0),fontHandle);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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

void SceneGame::StartGameOver(GameOverSource source)
{
	if (!isGameOver_)
	{
		isGameOver_ = true;
		gameOverTimer_ = 0;
		gameOverSource_ = source;

		// 例：ここで原因ごとの演出を分ける
		switch (gameOverSource_)
		{
		case GameOverSource::FOOD:
			// 餌切れ → 「ネコが怒っている」画像を出すとか
			// PlaySoundMem(foodAlertSE, DX_PLAYTYPE_BACK);
			break;

		case GameOverSource::WALL:
			// 壁破損 → 「警報」点滅演出など
			// PlaySoundMem(alarmSE, DX_PLAYTYPE_BACK);
			break;

		default:
			break;
		}
	}
}

void SceneGame::DrawInfo()
{
	if (isGameOver_) return;

	if (wall_->GetFlagImg() && wall_->GetIsMouseOver())
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 40, 0xffffff, "壁に穴が開いている。異常だ。");
	else if (wall_->GetIsMouseOver())
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 40, 0xffffff, "ただの壁だ。");

	if (food_->GetFlag() && food_->GetIsMouseOver())
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 40, 0xffffff, "餌が無くなっている。補充してあげよう。");
	else if (food_->GetIsMouseOver())
		DrawFormatString(0, Application::SCREEN_SIZE_Y - 40, 0xffffff, "餌はまだ残っている。");
}
