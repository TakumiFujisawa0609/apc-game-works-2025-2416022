
#include "Food.h"
#include "../../Application.h"
#include <DxLib.h>
#include <cmath>

Food::Food()
{
    type_ = ItemType::Food;
    graphHandle_ = -1;
    lifeTimer_ = 0;
    maxLifeTime_ = 600; // 約10秒で消える（=ゲームオーバー判定）
}

void Food::Init()
{
    // 餌画像を読み込み
    graphHandle_ = LoadGraph((Application::PATH_IMAGE + "5721.png").c_str());
    active_ = false;
}

void Food::Spawn(const VECTOR& pos)
{
    ItemBase::Spawn(pos);
    lifeTimer_ = 0;
}

void Food::Update()
{
    if (!active_) return;

    animFrame_++;
    lifeTimer_++;

    // ふわふわ上下に揺れる演出
    //pos_.y += sinf(animFrame_ * 0.05f) * 0.3f;

    // 時間経過で自動的に非アクティブ化してもOK（管理側がチェックしても可）
    if (lifeTimer_ > maxLifeTime_)
    {
        active_ = false;
    }
}

void Food::Draw()
{
    if (!active_ || graphHandle_ == -1) return;

    DrawGraph((int)pos_.x, (int)pos_.y, graphHandle_, TRUE);

    // デバッグ表示（残り寿命など）
     DrawFormatString((int)pos_.x, (int)pos_.y - 16, GetColor(255,255,255), "Life:%d", maxLifeTime_ - lifeTimer_);
}

bool Food::IsExpired() const
{
    return (lifeTimer_ > maxLifeTime_);
}
