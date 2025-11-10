
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Common/Vector2.h"
#include "TV.h"

TV::TV(void)
{
}

TV::~TV(void)
{
}

void TV::Init(void)
{
	img_ = LoadGraph((Application::PATH_ITEM + "nc304917.png").c_str());

	pos_.x = 700;
	pos_.y = 600;

	flag_ = false;

	isMouseOver_ = false;
}

void TV::Update(void)
{
    // --- 毎フレーム、マウスが壁の上にあるか判定する ---
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = TV_WID / 2.0f;
        float halfH = TV_HIG / 2.0f;

        isMouseOver_ =
            (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
                mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);
    }

    if (flagImg_ && InputManager::GetInstance().IsTrgMouseLeft())
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = TV_WID / 2.0f;
        float halfH = TV_HIG / 2.0f;

        if (isMouseOver_)
        {
            flagImg_ = false;
            flagSpawn_ = false;

            // 次の再表示までのランダム時間を短縮
            int randomOffset = rand() % 300;
            spawnTimer_ = (int)((spawnTimerBase_ + randomOffset) / spawnTimerMultiplier_);

            count_ = 0;
            return;
        }
    }


}

void TV::Draw(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.3, 0.0, img_, true);
}

void TV::Release(void)
{
    DeleteGraph(img_);
}

VECTOR TV::GetPos(void) const
{
	return pos_;
}

bool TV::GetFlag(void) const
{
	return false;
}

bool TV::GetIsMouseOver() const
{
	return false;
}
