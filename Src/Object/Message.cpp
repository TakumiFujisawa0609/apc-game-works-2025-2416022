
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
    isShowingImage_ = false;

    // --- キャンセルボタン ---
    cancelPos_ = { 900, 700 };
    cancelW_ = 170;
    cancelH_ = 40;

}

void Message::Update(void)
{
    if (isShowingImage_)
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();

        // --- キャンセルボタン判定 ---
        bool isCancelOver =
            (mousePos.x >= cancelPos_.x - cancelW_ / 2 && mousePos.x <= cancelPos_.x + cancelW_ / 2 &&
                mousePos.y >= cancelPos_.y - cancelH_ / 2 && mousePos.y <= cancelPos_.y + cancelH_ / 2);

        if (InputManager::GetInstance().IsTrgMouseLeft())
        {
            if (isCancelOver)
            {
                // キャンセル押下で中断
                isShowingImage_ = false;
                currentImageIndex_ = 0;
                return;
            }
            else
            {
                // 次の画像へ
                currentImageIndex_++;
                if (currentImageIndex_ >= (int)images_.size())
                {
                    // すべて表示し終えたら終了
                    isShowingImage_ = false;
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

    // 画像表示中なら画像を描画
    if (isShowingImage_)
    {
        // --- 現在の画像を描画 ---
        if (!images_.empty())
        {
            DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, imgB_, true);
            DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.8, 0.0, images_[currentImageIndex_], true);

        }

        // --- キャンセルボタン ---
        DrawBox(
            cancelPos_.x - cancelW_ / 2, cancelPos_.y - cancelH_ / 2,
            cancelPos_.x + cancelW_ / 2, cancelPos_.y + cancelH_ / 2,
            GetColor(57, 53, 48),
            true);

        Vector2 mousePos = InputManager::GetInstance().GetMousePos();

        if (mousePos.x >= cancelPos_.x - cancelW_ / 2 && mousePos.x <= cancelPos_.x + cancelW_ / 2 &&
            mousePos.y >= cancelPos_.y - cancelH_ / 2 && mousePos.y <= cancelPos_.y + cancelH_ / 2) {
        DrawBox(
            cancelPos_.x - cancelW_ / 2 , cancelPos_.y - cancelH_ / 2,
            cancelPos_.x + cancelW_ / 2 , cancelPos_.y + cancelH_ / 2,
            GetColor(255, 0, 0),
            false);}

        DrawFormatString(cancelPos_.x-60, cancelPos_.y - 10, GetColor(255, 255, 255), "メモ帳を閉じる");

        return;
    }

    if (isMouseOver_) {
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
}

bool Message::GetIsMouseOver() const
{
    return isMouseOver_;
}

bool Message::GetIsShowingImage() const
{
	return isShowingImage_;
}