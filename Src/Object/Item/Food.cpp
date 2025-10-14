
#include "Food.h"
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
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

    flag_ = false;
}

void Food::Update()
{
    // --- img_ クリック処理 ---
    if (flag_ && InputManager::GetInstance().IsTrgMouseLeft())
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = FOOD_WID * 0.3f / 2.0f;
        float halfH = FOOD_HIG * 0.3f / 2.0f;

        if (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
            mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH)
        {
            // img_ を非表示にして再表示タイマーを設定
            flag_ = false;

            // 次の再表示までのランダム時間を短縮
            int randomOffset = rand() % 300;
            spawnTimer_ = (int)((spawnTimerBase_ + randomOffset) / spawnTimerMultiplier_);

            // 再表示ごとに加速
            spawnTimerMultiplier_ *= 2.0f;

            count_ = 0;
            return;
        }
    }

    // --- spawnTimer_ により img_ 再表示 ---
    if (!flag_)
    {
        spawnTimer_--;
        if (spawnTimer_ <= 0)
        {
            flag_ = true;
            count_ = 0;
        }
    }
}


void Food::Draw(void)
{

    DrawRotaGraph(pos_.x, pos_.y, 0.03, 0.0, img_, true);

    /*DrawBox(pos_.x - FOOD_WID / 2, pos_.y - FOOD_HIG / 2,
        pos_.x + FOOD_WID / 2, pos_.y + FOOD_HIG / 2,
        GetColor(0, 255, 0), false);*/

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
    return flag_;
}
