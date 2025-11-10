
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include <DxLib.h>
#include <cmath>
#include "PC.h"

PC::PC(void)
{
}

PC::~PC(void)
{
}

void PC::Init(void)
{
	// 画像読み込み
	img_ = LoadGraph((Application::PATH_ITEM + "nc407592.png").c_str());

	pos_.x = 500;
	pos_.y = 600;

	flagImg_ = false;
	isMouseOver_ = false;
	isShowingInfo_ = false;

	infoImg_ = LoadGraph((Application::PATH_STAGE + "黒背景.png").c_str());
	infoText_ = "実績情報なし"; // デフォルト

	closePos_ = { 900, 700 };
	closeW_ = 170;
	closeH_ = 40;

	spawnTimerBase_ = 300;        // 基本再表示時間
	spawnTimerMultiplier_ = 1.0f; // 初期倍率
	spawnInterval_ = 300;
	spawnTimer_ = 180 + rand() % 300;
	count_ = 0;

	// 累積時間カウンタ
	activeTimer_ = 0;
	isGameOver_ = false;
}

void PC::Update(void)
{
	// --- 毎フレーム、マウスが上にあるか判定する ---
	{
		Vector2 mousePos = InputManager::GetInstance().GetMousePos();
		float halfW = PC_WID / 2.0f;
		float halfH = PC_HIG / 2.0f;
		isMouseOver_ =
			(mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
				mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);


		// --- 情報ウィンドウ表示中 ---
		if (isShowingInfo_)
		{
			bool isCloseOver =
				(mousePos.x >= closePos_.x - closeW_ / 2 && mousePos.x <= closePos_.x + closeW_ / 2 &&
					mousePos.y >= closePos_.y - closeH_ / 2 && mousePos.y <= closePos_.y + closeH_ / 2);

			if (InputManager::GetInstance().IsTrgMouseLeft() && isCloseOver)
			{
				// 閉じる
				isShowingInfo_ = false;
			}

			return;
		}

		// --- PCクリックでウィンドウを開く ---
		if (flagImg_ && InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_)
		{
			isShowingInfo_ = true;
		}

	}
}

void PC::Draw(void)
{
	// --- PC本体 ---
	if (flagImg_)
		DrawRotaGraph(pos_.x, pos_.y, 0.05, 0.0, img_, true);

	if (isMouseOver_)
	{
		float halfW = PC_WID / 2.0f;
		float halfH = PC_HIG / 2.0f;
		DrawBox(
			pos_.x - halfW, pos_.y - halfH,
			pos_.x + halfW, pos_.y + halfH,
			GetColor(255, 0, 0), false);
	}

	// --- 実績情報ウィンドウ ---
	if (isShowingInfo_)
	{
		// 背景
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, infoImg_, true);

		// テキスト描画（中央寄せ）
		int textW = GetDrawStringWidth(infoText_.c_str(), (int)infoText_.size());
		DrawFormatString((Application::SCREEN_SIZE_X - textW) / 2, Application::SCREEN_SIZE_Y / 2, GetColor(255, 255, 255), infoText_.c_str());

		// 閉じるボタン
		bool isOver = false;
		Vector2 mousePos = InputManager::GetInstance().GetMousePos();
		if (mousePos.x >= closePos_.x - closeW_ / 2 && mousePos.x <= closePos_.x + closeW_ / 2 &&
			mousePos.y >= closePos_.y - closeH_ / 2 && mousePos.y <= closePos_.y + closeH_ / 2)
			isOver = true;

		DrawBox(
			closePos_.x - closeW_ / 2, closePos_.y - closeH_ / 2,
			closePos_.x + closeW_ / 2, closePos_.y + closeH_ / 2,
			GetColor(57, 53, 48), true);

		if (isOver)
		{
			DrawBox(
				closePos_.x - closeW_ / 2, closePos_.y - closeH_ / 2,
				closePos_.x + closeW_ / 2, closePos_.y + closeH_ / 2,
				GetColor(255, 0, 0), false);
		}

		DrawFormatString(closePos_.x - 60, closePos_.y - 10, GetColor(255, 255, 255), "閉じる");
	}
}

void PC::Release(void)
{
	DeleteGraph(img_);
	DeleteGraph(infoImg_);
}

void PC::SetInfoText(const std::string& text)
{
	infoText_ = text;
}

VECTOR PC::GetPos(void) const
{
	return pos_;
}

bool PC::GetFlag(void) const
{
	return flag_;
}

bool PC::GetIsMouseOver() const
{
	return isMouseOver_;
}
