
#include <DxLib.h>

#include "Message.h"
#include "../Manager/Generic/InputManager.h"
#include "../DrawUI/Font.h"
#include "../Application.h"

Message::Message(void)
{
}

Message::~Message(void)
{
}

void Message::Init(void)
{

	pos_.x = 200;
	pos_.y = 200;

	isMouseOver_ = false;
	flagImg_ = true;

    // 画像読み込み
    images_.push_back(LoadGraph((Application::PATH_TEXT + "nc111382.png").c_str()));
	images_.push_back(LoadGraph((Application::PATH_TEXT + "nc111380.png").c_str()));

	imgA_ = LoadGraph((Application::PATH_TEXT + "nc111382.png").c_str());
	imgB_ = LoadGraph((Application::PATH_STAGE + "黒背景.png").c_str());
    isShowingImage_ = false;
    currentImageIndex_ = 0;

    // --- キャンセルボタン ---
    cancelPos_ = { 900, 700 };
    cancelW_ = 170;
    cancelH_ = 40;

	// --- 次へボタン ---
    nextPos_ = { 820, Application::SCREEN_SIZE_Y / 2 };
    nextW_ = 120;
    nextH_ = 40;

	// --- 戻るボタン ---
    backPos_ = { 200, Application::SCREEN_SIZE_Y / 2 };
    backW_ = 120;
    backH_ = 40;

}

void Message::Update(void)
{
    if (isShowingImage_)
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();

        // --- 各ボタンの範囲判定 ---
        bool isCancelOver =
            (mousePos.x >= cancelPos_.x - cancelW_ / 2 && mousePos.x <= cancelPos_.x + cancelW_ / 2 &&
                mousePos.y >= cancelPos_.y - cancelH_ / 2 && mousePos.y <= cancelPos_.y + cancelH_ / 2);

        bool isNextOver =
            (mousePos.x >= nextPos_.x - nextW_ / 2 && mousePos.x <= nextPos_.x + nextW_ / 2 &&
                mousePos.y >= nextPos_.y - nextH_ / 2 && mousePos.y <= nextPos_.y + nextH_ / 2);

        bool isBackOver =
            (mousePos.x >= backPos_.x - backW_ / 2 && mousePos.x <= backPos_.x + backW_ / 2 &&
                mousePos.y >= backPos_.y - backH_ / 2 && mousePos.y <= backPos_.y + backH_ / 2);

        if (InputManager::GetInstance().IsTrgMouseLeft())
        {
            if (isCancelOver)
            {
                // キャンセル押下で中断
                isShowingImage_ = false;
                currentImageIndex_ = 0;
                return;
            }
            else if (isNextOver)
            {
                // 次ページ
                currentImageIndex_++;
                if (currentImageIndex_ >= (int)images_.size())
                {
                    isShowingImage_ = false;
                    currentImageIndex_ = 0;
                }
            }
            else if (isBackOver)
            {
                // 前ページ
                currentImageIndex_--;
                if (currentImageIndex_ < 0)
                {
                    currentImageIndex_ = 0;
                }
            }
        }

        return; // 表示中は他の処理を止める
    }

    // --- 毎フレーム、マウスが壁の上にあるか判定する ---
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();

        isMouseOver_ =
            (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
                mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);
    }

    if (flagImg_ && InputManager::GetInstance().IsTrgMouseLeft())
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();

        if (isMouseOver_)
        {
			isShowingImage_ = true;
            currentImageIndex_ = 0; // 最初の画像から開始
        }
    }
}

void Message::Draw(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.08, 0.0, imgA_, true);

    // --- 表示中 ---
    if (isShowingImage_)
    {
        if (!images_.empty())
        {
            DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, imgB_, true);
            DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.8, 0.0, images_[currentImageIndex_], true);
        }

        Vector2 mousePos = InputManager::GetInstance().GetMousePos();

        // --- 汎用関数風に描画まとめ ---
        auto drawButton = [&](VECTOR pos, int w, int h, const char* text, bool isOver) {
            DrawBox(
                pos.x - w / 2, pos.y - h / 2,
                pos.x + w / 2, pos.y + h / 2,
                GetColor(57, 53, 48),
                true);
            if (isOver)
            {
                DrawBox(
                    pos.x - w / 2, pos.y - h / 2,
                    pos.x + w / 2, pos.y + h / 2,
                    GetColor(255, 0, 0),
                    false);
            }
            int textX = pos.x - GetDrawStringWidth(text, strlen(text)) / 2;
            DrawFormatString(textX, pos.y - 10, GetColor(255, 255, 255), text);
            };

        // --- 各ボタンのホバー判定 ---
        bool overCancel = (mousePos.x >= cancelPos_.x - cancelW_ / 2 && mousePos.x <= cancelPos_.x + cancelW_ / 2 &&
            mousePos.y >= cancelPos_.y - cancelH_ / 2 && mousePos.y <= cancelPos_.y + cancelH_ / 2);
        bool overNext = (mousePos.x >= nextPos_.x - nextW_ / 2 && mousePos.x <= nextPos_.x + nextW_ / 2 &&
            mousePos.y >= nextPos_.y - nextH_ / 2 && mousePos.y <= nextPos_.y + nextH_ / 2);
        bool overBack = (mousePos.x >= backPos_.x - backW_ / 2 && mousePos.x <= backPos_.x + backW_ / 2 &&
            mousePos.y >= backPos_.y - backH_ / 2 && mousePos.y <= backPos_.y + backH_ / 2);

        // --- 各ボタン描画 ---
        drawButton(backPos_, backW_, backH_, "← 前へ", overBack);
        drawButton(nextPos_, nextW_, nextH_, "次へ →", overNext);
        drawButton(cancelPos_, cancelW_, cancelH_, "メモ帳を閉じる", overCancel);

        return;
    }

    // --- 通常状態でマウスオーバー枠 ---
    if (isMouseOver_)
    {
        DrawBox(
            pos_.x - halfW, pos_.y - halfH,
            pos_.x + halfW, pos_.y + halfH,
            GetColor(255, 0, 0), false);
    }
}

void Message::Release(void)
{
    for (auto& img : images_)
        DeleteGraph(img);

	DeleteGraph(imgA_);
	DeleteGraph(imgB_);
}

bool Message::GetIsMouseOver() const
{
    return isMouseOver_;
}

bool Message::GetIsShowingImage() const
{
	return isShowingImage_;
}