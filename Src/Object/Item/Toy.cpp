
#include "Toy.h"
#include "../../Application.h"
#include <DxLib.h>
#include <cmath>

Toy::Toy(void)
{
    lifeTimer_ = 0;
    maxLifeTime_ = 600; // 約10秒で消える（=ゲームオーバー判定）
}

Toy::~Toy(void)
{
}

void Toy::Init(void)
{
    // 餌画像を読み込み
    img_ = LoadGraph((Application::PATH_IMAGE + "5721.png").c_str());
    flag_ = false;
}

void Toy::Update(void)
{
    if (!flag_) return;

    lifeTimer_++;

    // ふわふわ上下に揺れる演出
    //pos_.y += sinf(animFrame_ * 0.05f) * 0.3f;

    // 時間経過で自動的に非アクティブ化してもOK（管理側がチェックしても可）
    if (lifeTimer_ > maxLifeTime_)
    {
        flag_ = false;
    }
}

void Toy::Draw(void)
{
    if (!flag_ || img_ == -1) return;

    DrawGraph(pos_.x, pos_.y, img_, TRUE);

    // デバッグ表示（残り寿命など）
    DrawFormatString((int)pos_.x, (int)pos_.y - 16, GetColor(255, 255, 255), "Life:%d", maxLifeTime_ - lifeTimer_);
}

void Toy::Release(void)
{
    DeleteGraph(img_);
}

bool Toy::GetFlag(void) const
{
    return flag_;
}
