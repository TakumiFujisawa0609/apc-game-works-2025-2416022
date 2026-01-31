#include "SceneGame.h"

#include <DxLib.h>

#include "../Object/Stage/Stage.h"
#include "../Object/Neko/Neko.h"
#include "../Object/Item/Food.h"
#include "../Object/Item/Wall.h"
#include "../Object/Item/Toy.h"
#include "../Object/Item/PC.h"
#include "../Object/Item/TV.h"
#include "../Object/Item/Book.h"

#include "../Object/Message.h"
#include "../Common/Easing.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Audio/AudioManager.h"

SceneGame::SceneGame(void)
{
	neko_ = nullptr;
	stage_ = nullptr;
	food_ = nullptr;
	wall_ = nullptr;
	toy_ = nullptr;
	pc_ = nullptr;
	tv_ = nullptr;
	book_ = nullptr;

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

	toy_ = new Toy();
	toy_->Init();

	pc_ = new PC();
	pc_->Init();

	tv_ = new TV();
	tv_->Init();

	book_ = new Book();
	book_->Init();

	message_ = new Message();
	message_->Init();

	count_ = 0;
	isEnd_ = false;

	neko_->SetFood(food_);
	neko_->SetPC(pc_);
	neko_->SetTV(tv_);
	neko_->SetBook(book_);

	img3_ = LoadGraph((Application::PATH_ITEM + "nc296608.png").c_str());
	img4_ = LoadGraph((Application::PATH_STAGE + "黒背景.png").c_str());
	img5_ = LoadGraph((Application::PATH_STAGE + "白円.png").c_str());
	img6_ = LoadGraph((Application::PATH_ITEM + "NyanCat.png").c_str());
	img7_ = LoadGraph((Application::PATH_ITEM + "画面4.png").c_str());
	img8_ = LoadGraph((Application::PATH_ITEM + "nc304917.png").c_str());

	for (int i = 0; i < 8; ++i) {
		std::string filename = "ぬいぐるみ" + std::to_string(i + 1) + ".png"; // フレーム1から始まる場合
		toyAnimationHandles_[i] = LoadGraph((Application::PATH_ITEM + filename).c_str());
		if (toyAnimationHandles_[i] == -1) {
			// ロード失敗時のエラー処理
			// DebugBreak(); またはログ出力
		}
	}
	currentAnimationFrame_ = 0;

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);
	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_GAME);

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
		case GameOverSource::TOY:
			toy_->Update();
			break;
		case GameOverSource::PC:
			pc_->Update();
			break;
		case GameOverSource::TV:
			tv_->Update();
			break;
		case GameOverSource::Book:
			book_->Update();
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

	// メッセージが画像表示中なら他を止める
	if (message_->GetIsShowingImage())
	{
		message_->Update(); // 画像閉じる処理だけ続行
		return;
	}

	if (pc_->GetFlagImg())
	{
		pc_->Update();
		return; 
	}

	// --- 通常時の更新 ---
	stage_->Update();
	neko_->Update();
	food_->Update();
	wall_->Update();
	toy_->Update(food_);
	pc_->Update();
	tv_->Update();
	book_->Update();

	tv_->SetNekoPos(neko_->GetPos());
	pc_->SetNekoPos(neko_->GetPos());

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

	// --- Toy ゲームオーバー判定 ---
	if (toy_->IsGameOver())
	{
		StartGameOver(GameOverSource::TOY);
		return;
	}

	// --- PC ゲームオーバー判定 ---
	if (pc_->IsGameOver())
	{
		StartGameOver(GameOverSource::PC);
		return;
	}

	// --- TV ゲームオーバー判定 ---
	if (tv_->IsGameOver())
	{
		StartGameOver(GameOverSource::TV);
		return;
	}

	// Book
	if (book_->IsGameOver())
	{
		StartGameOver(GameOverSource::Book);
		return;
	}

	// --- ゲームクリアタイマー ---
	count_++;
	int clearTime = Application::isHardMode ? 7200 : 3600;

	if (count_ >= clearTime)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
		return;
	}
}


void SceneGame::Draw(void)
{
	

	stage_->Draw();
	neko_->Draw();
	food_->Draw();
	wall_->Draw();
	toy_->Draw();
	pc_->Draw();
	tv_->Draw();
	book_->Draw();

	message_->Draw();

	DrawInfo();

	// --- 実績ウィンドウが開いている場合は最前面に描画 ---
	if (pc_->GetFlagImg()) {
		pc_->DrawAchievementWindow();
	}

	if (tv_ && tv_->IsGamePlaying() && tv_->GetMinigame())
	{
		// 他の要素の上に描画するため、TVのDraw()呼び出し後に配置する
		// ミニゲームの共通描画 (背景、枠) と独自のゲーム内容が描画される
		tv_->GetMinigame()->Draw();
	}


	if (isGameOver_)
	{
		if (gameOverSource_ == GameOverSource::FOOD)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2,
				1.0, 0.0, img4_, true);


			DrawRotaGraph(300, 600, 0.7, 0.0, img5_, true);

			DrawStringToHandle(0, Application::SCREEN_SIZE_Y - 40, "食べ過ぎには注意しましょう。ネコの管理もあなたの仕事です。", GetColor(255, 0, 0), fontHandle);
		}
		else if (gameOverSource_ == GameOverSource::WALL)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2,
				1.0, 0.0, img4_, true);

			// --- ゲームオーバー画像 ---
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2,
				1.0, 0.0, img3_, true);

			DrawStringToHandle(0, Application::SCREEN_SIZE_Y - 40, "壁の穴を放置してはいけません。修繕費用はあなた持ちです。", GetColor(255, 0, 0), fontHandle);
		}
		else if (gameOverSource_ == GameOverSource::TOY)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2,
				1.0, 0.0, img4_, true);
			DrawRotaGraph(300, 600, 1.0, 0.0, img5_, true);

			// アニメーションを1秒（60フレーム）で完結させる
			const int ANIMATION_DURATION = 60; // 1秒 = 60フレーム
			const int NUM_FRAMES = 4;
			// 1フレームあたりの表示時間 (60フレーム / 8フレーム = 7.5フレーム/枚)
			int framePerImage = ANIMATION_DURATION / NUM_FRAMES;
			// 現在のフレームを計算 (0から7)
			int currentFrame = gameOverTimer_ / framePerImage;
			// フレームが総フレーム数を超えないように制限
			if (currentFrame >= NUM_FRAMES)
			{
				currentFrame = NUM_FRAMES - 1; // 最後のフレーム（7）で固定
			}
			// アニメーションフレームを描画
			DrawRotaGraph(300, 600, 0.1, 0.0, toyAnimationHandles_[currentFrame], true);

			const int GAMEOVER_WAIT_ASSUMED = 180; // 仮の値
			const int STOP_BLINK_TIME = GAMEOVER_WAIT_ASSUMED - 60; // 120フレーム

			// --- 1. 点滅処理 (STOP_BLINK_TIME 未満の場合のみ実行) ---
			if (gameOverTimer_ < STOP_BLINK_TIME) // 120フレームまで点滅
			{
				const int BLINK_SPEED = 5;
				int blinkState = (gameOverTimer_ / BLINK_SPEED) % 2;

				if (blinkState == 1)
				{
					const int BLACK_COLOR = GetColor(0, 0, 0);
					DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, BLACK_COLOR, TRUE);
				}
			}


			// --- 2. 残り1秒（STOP_BLINK_TIME 以降）の画像描画 ---
			if (gameOverTimer_ >= STOP_BLINK_TIME) // 120フレーム以降に追加画像を描画
			{
				DrawRotaGraph(
					Application::SCREEN_SIZE_X / 2,
					Application::SCREEN_SIZE_Y / 2,
					4.0, 0.0, toyAnimationHandles_[7], true);
			}

			DrawStringToHandle(0, Application::SCREEN_SIZE_Y - 40, "おもちゃを放置してはいけません。特に上から落ちてくるようなおもちゃを。", GetColor(255, 0, 0), fontHandle);
		}
		else if (gameOverSource_ == GameOverSource::PC)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2,
				1.0, 0.0, img4_, true);
			// --- ゲームオーバー画像 ---
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2,
				2.0, 0.0, img6_, true);
			DrawStringToHandle(0, Application::SCREEN_SIZE_Y - 40, "YOUR COMPUTER HAS BEEN FUCKED BY THE MEMZ TROJAN.Your computer won't boot up again, so use it as long as you can!:D", GetColor(255, 0, 0), fontHandle);
		}
		else if (gameOverSource_ == GameOverSource::TV)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2,
				1.0, 0.0, img4_, true);
			//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			// --- ゲームオーバー画像 ---
			DrawRotaGraph(700, 450, 1.0, 0.0, img5_, true);
			DrawRotaGraph(700, 450, 0.1, 0.0, img7_, true);
			DrawRotaGraph(700, 450, 0.1, 0.0, img8_, true);
			//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawStringToHandle(0, Application::SCREEN_SIZE_Y - 40, "もう手遅れだ。", GetColor(255, 0, 0), fontHandle);
		}
		else if (gameOverSource_ == GameOverSource::Book)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
				Application::SCREEN_SIZE_Y / 2,
				1.0, 0.0, img4_, true);
			DrawRotaGraph(300, 450, 1.0, 0.0, img5_, true);

			DrawStringToHandle(0, Application::SCREEN_SIZE_Y - 40, "本棚の整理を怠ってはいけません。部屋の管理があなたの仕事です。", GetColor(255, 0, 0), fontHandle);
		}
	}

	DrawFormatString(0, 0, 0x000000, "異常を発見したらクリックで対処してください。");
	if (Application::isHardMode && !isGameOver_) {
		DrawString(0, 20, "MODE: HARD (Time x2)", GetColor(255, 0, 0));
	}

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

	if (toy_)
	{
		toy_->Release();
		delete toy_;
		toy_ = nullptr;
	}

	if (pc_)
	{
		pc_->Release();
		delete pc_;
		pc_ = nullptr;
	}

	if (tv_)
	{
		tv_->Release();
		delete tv_;
		tv_ = nullptr;
	}

	if (book_)
	{
		book_->Release();
		delete book_;
		book_ = nullptr;
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

		case GameOverSource::TOY:
			// おもちゃ放置 → 「ネコが怖がっている」画像を出すとか
			// PlaySoundMem(toyAlertSE, DX_PLAYTYPE_BACK);
			break;

		case GameOverSource::PC:
			// 仕事放置 → 「上司からの怒り」画像を出すとか
			// PlaySoundMem(pcAlertSE, DX_PLAYTYPE_BACK);
			break;

		case GameOverSource::TV:
			// テレビ放置 → 「ネコが退屈している」画像を出すとか
			// PlaySoundMem(tvAlertSE, DX_PLAYTYPE_BACK);
			break;

		case GameOverSource::Book:
			break;

		default:
			break;
		}
	}
}

void SceneGame::DrawInfo()
{
	if (isGameOver_) return;

	// 表示用のメッセージを一時的に格納
	const char* infoText = nullptr;

	// 優先順位の高いものから順にチェック

	if (pc_->GetIsMouseOver())
	{
		if (pc_->GetFlag())
			infoText = "コンピュータウイルスに感染している…！";
		else
			infoText = "パソコンです。実績が確認できます。";
	}

	else if (message_->GetIsMouseOver())
	{
		infoText = "メモ帳です。前任者のメモが確認できます。";
	}
	else if (toy_->GetIsMouseOver())
	{

		if (toy_->IsEating())
			infoText = "おもちゃが餌を食べている！";
		else if (toy_->IsFalling() || (toy_->GetFlagShadow() && !toy_->IsLanded()))
			infoText = "…？";
		else if (toy_->IsLanded())
			infoText = "おもちゃが放置されている…？";
		
	}
	else if (food_->GetIsMouseOver())
	{
		if (food_->GetFlag())
			infoText = "餌は補充されている。";
		else
			infoText = "餌はない。";
	}
	else if (wall_->GetIsMouseOver())
	{
		if (wall_->GetFlagImg())
			infoText = "壁に穴が開いている…。";
		else
			infoText = "ただの壁だ。";
	}
	else if (tv_->GetIsMouseOver())
	{
		if (!tv_->GetFlag())
		{
			infoText = "空のカラーテレビだ。壊れているため、電源を点けることはできない。";
		}
		else
		{
			int level = 0;
			tv_->GetFlagLevel(level);

			switch (level)
			{
			case 0:
				infoText = "テレビが点いている…？";
				break;
			case 1:
				infoText = "ノイズが走っている…。";
				break;
			case 2:
				infoText = "ニュース画面だ。天気予報のようだが、ここでは必要ない。";
				break;
			case 3:
				infoText = "首のない鶏が映っている。この鶏はこの状態で18ヶ月間生きたそうだ。";
				break;
			case 4:
				infoText = "……。";
				break;
			default: // level >= 5
				break;
			}
		}
	}

	else if (book_->GetIsMouseOver())
	{
		if (book_->GetTargetImg())
			infoText = "良く整理された本棚だ。";
		else
			infoText = "本棚が整理されていない。ネコが来るまでに本棚を整理しておこう。";
	}

	else if (neko_->GetIsMouseOver())
	{
		infoText = "ネコだ。";
		// if (neko_->IsMoving()) infoText = "ネコが動いている。";
	}

	// --- 表示 ---
	if (infoText)
	{
		DrawFormatString(
			0,
			Application::SCREEN_SIZE_Y - 40,
			GetColor(0, 0, 0),
			"%s", infoText);
	}
}