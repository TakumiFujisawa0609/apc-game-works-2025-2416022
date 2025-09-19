#include "SceneManager.h"

#include <chrono>
#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include<cassert>

#include "../../Scene/SceneTitle.h"
#include "../../Scene/SceneGame.h"
#include "../../Scene/SceneGameOver.h"
#include "../../Scene/SceneGameClear.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	//各マネジャーの生成


	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	mosaicfade_ = std::make_unique<MosaicFade>();
	mosaicfade_->Init();

	//シーン
	scene_ = new SceneTitle();
	scene_->Init();

	isSceneChanging_ = false;

	//デルタタイム

	preTime_ = std::chrono::system_clock::now();

	//初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Update(void)
{
	if (scene_ == nullptr)
	{
		return;
	}

	//デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	mosaicfade_ -> Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

}

void SceneManager::Draw(void)
{
	//描画先グラフィック領域の指定
	//(3D描画でしようするカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);
	
	//フロントバッファの画像を消去
	ClearDrawScreen();
	

	//ゲーム内容描画
	//描画
	scene_->Draw();

	//フェード
	mosaicfade_->Draw();
}

void SceneManager::Destroy(void)
{
	scene_->Release();
	delete scene_;

	delete instance_;
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	//フェードアウトを開始する
	mosaicfade_->SetFade(MosaicFade::STATE::FADE_OUT);
	isSceneChanging_ = true;
}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void)const
{
	return deltaTime_;
}


SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	mosaicfade_ = nullptr;

	isSceneChanging_ = false;

	//デルタタイム
	deltaTime_ = 1.0f / 60.0f;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 1.0f / 60.0f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	//リソースの解放


	//シーンを変更する
	sceneId_ = sceneId;

	//現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	switch (sceneId_)
	{
	case SceneManager::SCENE_ID::TITLE:
		scene_ = new SceneTitle();
		break;

	case SceneManager::SCENE_ID::GAME:
		scene_ = new SceneGame();
		break;

	case SceneManager::SCENE_ID::GAMECLEAR:
		scene_ = new SceneGameClear();
		break;

	case SceneManager::SCENE_ID::GAMEOVER:
		scene_ = new SceneGameOver();
		break;
	}

	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::Fade(void)
{
	MosaicFade::STATE fState = mosaicfade_->GetState();
	switch (fState)
	{
	case MosaicFade::STATE::FADE_IN:
		// 明転中
		if (mosaicfade_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			mosaicfade_->SetFade(MosaicFade::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case MosaicFade::STATE::FADE_OUT:
		// 暗転中
		if (mosaicfade_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			mosaicfade_->SetFade(MosaicFade::STATE::FADE_IN);
		}
		break;
	}
}

