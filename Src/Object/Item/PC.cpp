#include "PC.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Application.h"
#include <DxLib.h>
#include <cstdlib>

PC::PC()
    : flag_(false), flagImg_(false), isMouseOver_(false),
    spawnInterval_(300), spawnTimer_(0), count_(0), activeTimer_(0), isGameOver_(false),
    img_(0), infoImg_(0), closeW_(170), closeH_(40)
{
}

PC::~PC()
{
}

void PC::Init()
{
    img_ = LoadGraph((Application::PATH_ITEM + "nc215732.png").c_str());
    img2_ = LoadGraph((Application::PATH_ITEM + "nc316039.png").c_str());
	img3_ = LoadGraph((Application::PATH_ITEM + "NyanCat.png").c_str());
    infoImg_ = LoadGraph((Application::PATH_STAGE + "黒背景.png").c_str());
    infoText_ = "実績情報なし";

    pos_ = { 85, 400 };
    closePos_ = { 900, 700 };
    cancelPos_ = { 0, 0 };
    cancelW_ = cancelH_ = 0;

    spawnTimer_ = rand() % spawnInterval_;

	flagLevel_ = 0;
}

void PC::Update()
{
    Vector2 mousePos = InputManager::GetInstance().GetMousePos();

    // --- マウスオーバー判定 ---
    float halfW = PC_WID / 2.0f;
    float halfH = PC_HIG / 2.0f;
    isMouseOver_ =
        (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
            mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);

    // --- ランダムでアクティブに ---
    if (!flagImg_ && !flag_) {
        spawnTimer_--;
        if (spawnTimer_ <= 0) {
            flag_ = true;  // PCをアクティブにする
            spawnTimer_ = spawnInterval_ + rand() % spawnInterval_;
        }
    }

    // --- 実績ウィンドウ表示中 ---
    if (flagImg_) {
        bool isCloseOver =
            (mousePos.x >= closePos_.x - closeW_ / 2 && mousePos.x <= closePos_.x + closeW_ / 2 &&
                mousePos.y >= closePos_.y - closeH_ / 2 && mousePos.y <= closePos_.y + closeH_ / 2);

        if (InputManager::GetInstance().IsTrgMouseLeft() && isCloseOver) {
            flagImg_ = false;
			if (flag_)
				flag_ = false;
        }
        return;
    }

    // --- PCクリックでウィンドウを開く ---
    if (InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_) {
        flagImg_ = true;
    }

    // --- PCがアクティブならネコが接近 ---
    if (flag_) {
        // 距離計算
        float dx = nekoPos_.x - (pos_.x);
        float dy = nekoPos_.y - (pos_.y + 200.0f); // GetTargetPosと同じ補正
        float dist = sqrtf(dx * dx + dy * dy);


        if (dist < 150.0f) { // 接近判定範囲
            progressTimer_++;
            if (progressTimer_ > 180) { // 約3秒で段階進行
                if (flagLevel_ < maxLevel_)
                    flagLevel_++;
                progressTimer_ = 0;
            }
        }
        else {
            progressTimer_ = 0;
        }
    }
    else {
        progressTimer_ = 0;
    }

    // --- ゲームオーバー判定 ---
    if (flagLevel_ >= maxLevel_) {
        isGameOver_ = true;
    }
}



void PC::Draw(void)
{
    // 通常のPCアイコン描画
    DrawRotaGraph(pos_.x, pos_.y, 0.05, 0.0, img_, true);
	DrawRotaGraph(pos_.x-100, pos_.y+150, 0.1, 0.0, img2_, true);

    if (isMouseOver_) {
        DrawBox(
            pos_.x - PC_WID / 2.0f, pos_.y - PC_HIG / 2.0f,
            pos_.x + PC_WID / 2.0f, pos_.y + PC_HIG / 2.0f,
            GetColor(255, 0, 0), false);
    }
}

// 実績ウィンドウ専用描画
void PC::DrawAchievementWindow(void)
{
    if (!flagImg_) return;

    // 実績ウィンドウ背景
    DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, infoImg_, true);

    // テキスト or 画像切り替え
    if (flag_) {
        DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.5,0.0,img3_, true);
    }
    else {
        int textW = GetDrawStringWidth(infoText_.c_str(), (int)infoText_.size());
        DrawFormatString((Application::SCREEN_SIZE_X - textW) / 2, Application::SCREEN_SIZE_Y / 2,
            GetColor(255, 255, 255), infoText_.c_str());
    }

    // 閉じるボタン
    bool isOver = (InputManager::GetInstance().GetMousePos().x >= closePos_.x - closeW_ / 2 &&
        InputManager::GetInstance().GetMousePos().x <= closePos_.x + closeW_ / 2 &&
        InputManager::GetInstance().GetMousePos().y >= closePos_.y - closeH_ / 2 &&
        InputManager::GetInstance().GetMousePos().y <= closePos_.y + closeH_ / 2);

    DrawBox(closePos_.x - closeW_ / 2, closePos_.y - closeH_ / 2,
        closePos_.x + closeW_ / 2, closePos_.y + closeH_ / 2, GetColor(57, 53, 48), true);

    if (isOver)
        DrawBox(closePos_.x - closeW_ / 2, closePos_.y - closeH_ / 2,
            closePos_.x + closeW_ / 2, closePos_.y + closeH_ / 2, GetColor(255, 0, 0), false);

    DrawFormatString(closePos_.x - 60, closePos_.y - 10, GetColor(255, 255, 255), "閉じる");
}



void PC::Release()
{
    DeleteGraph(img_);
    DeleteGraph(img2_);
    DeleteGraph(infoImg_);
}

void PC::SetInfoText(const std::string& text)
{
    infoText_ = text;
}

void PC::ChangeImage()
{
    img_ = LoadGraph((Application::PATH_ITEM + "pc_on.png").c_str());
}

VECTOR PC::GetPos() const
{
    return pos_;
}

VECTOR PC::GetTargetPos() const
{
    VECTOR target = pos_;
    target.y += 200; // ネコの立ち位置例
    return target;
}

bool PC::GetFlag() const
{
    return flag_;
}

bool PC::GetIsMouseOver() const
{
    return isMouseOver_;
}
