
#include "Food.h"
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include <DxLib.h>
#include <cmath>

Food::Food(void)
{
    
}

Food::~Food(void)
{
}

void Food::Init(void)
{
    // 餌画像を読み込み
    img_ = LoadGraph((Application::PATH_ITEM + "5721.png").c_str());
    
    pos_.x = 300;
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

void Food::Update()
{
    // --- 毎フレーム、マウスが上にあるか判定する ---
    Vector2 mousePos = InputManager::GetInstance().GetMousePos();
    float halfW = FOOD_WID / 2.0f;
    float halfH = FOOD_HIG / 2.0f;

    isMouseOver_ =
        (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
            mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);

    // --- クリックで flagImg_ を反転 ---
    if (isMouseOver_ && InputManager::GetInstance().IsTrgMouseLeft())
    {
        flagImg_ = !flagImg_;  // true ⇄ false を切り替え
        activeTimer_ = 0;      // 必要なら累積タイマーをリセット
    }

    // 表示中なら累積タイマー加算
    if (flagImg_)
    {
        activeTimer_++;

        // 制限時間を超えたらゲームオーバー
        if (activeTimer_ > activeLimit_)
        {
            //isGameOver_ = true;
        }
    }
}



void Food::Draw(void)
{

    DrawRotaGraph(pos_.x, pos_.y, 0.03, 0.0, img_, true);

    float halfW = FOOD_WID / 2.0f;
    float halfH = FOOD_HIG / 2.0f;

    // --- img_ の描画 ---
    if (flagImg_)
    {
        DrawRotaGraph(pos_.x , pos_.y, 0.05, 0.0, img_, true);
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

void Food::Release(void)
{
    DeleteGraph(img_);
}

VECTOR Food::GetPos(void) const
{
    return pos_;
}

bool Food::GetFlag(void) const
{
    return flagImg_;
}

bool Food::GetIsMouseOver() const
{
    return isMouseOver_;
}

void Food::ChangeImage()
{
	// 新しい画像に切り替え
    //img_ = LoadGraph((Application::PATH_ITEM + "food_empty.png").c_str());
}
