
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
	}

	// --- img_ クリック処理 ---
	if (flagImg_ && InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_)
	{
		Vector2 mousePos = InputManager::GetInstance().GetMousePos();
		float halfW = PC_WID / 2.0f;
		float halfH = PC_HIG / 2.0f;
		if (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
			mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH)
		{
			// img_ を非表示にして再表示タイマーを設定
			flagImg_ = false;
			activeTimer_ = 0;
			// 完全ランダムな再出現タイマー設定（例：3〜8秒）
			spawnTimer_ = 1200 + rand() % 300; // 180〜480フレーム（約3〜8秒）
			// 再表示間隔を固定（短縮しない）
			spawnTimerMultiplier_ = 1.0f;
			count_ = 0;
			return;
		}
	}
}

void PC::Draw(void)
{
	DrawRotaGraph(pos_.x, pos_.y, 0.05, 0.0, img_, true);
	float halfW = PC_WID / 2.0f;
	float halfH = PC_HIG / 2.0f;
	// --- img_ の描画 ---
	if (flagImg_)
	{
		DrawRotaGraph(pos_.x, pos_.y, 0.05, 0.0, img_, true);
		/*DrawBox(
			pos_.x - halfW, pos_.y - halfH,
			pos_.x + halfW, pos_.y + halfH,
			GetColor(0, 0, 255), false);*/
	}
	int color = isMouseOver_ ? GetColor(255, 0, 0) : GetColor(0, 255, 0);
	if (isMouseOver_) {
		DrawBox(
			pos_.x - halfW, pos_.y - halfH,
			pos_.x + halfW, pos_.y + halfH,
			color, false);
	}
}

void PC::Release(void)
{
	DeleteGraph(img_);
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
