
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
}

void Message::Update(void)
{
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

        if (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
            mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH)
        {

            return;
        }
    }
}

void Message::Draw(void)
{
    DrawBox(
        pos_.x - halfW, pos_.y - halfH,
        pos_.x + halfW, pos_.y + halfH,
        GetColor(255, 0, 0), false);
}

void Message::Release(void)
{
}

bool Message::GetIsMouseOver() const
{
    return isMouseOver_;
}
