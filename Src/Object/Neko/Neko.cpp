
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "Neko.h"
#include <DxLib.h>


Neko::Neko(void)
{
}

Neko::~Neko(void)
{
}

void Neko::Init(void)
{
	img_= LoadGraph((Application::PATH_MODEL + "Nekoproto.jpg").c_str());

	pos_.x = Application::SCREEN_SIZE_X / 2;
	pos_.y = Application::SCREEN_SIZE_Y / 2+100;

	moveTimer_ = 0;
	isMoving_ = false;
	isMouseOver_ = false;
	targetType_ = TARGET::NONE;

    ChangeState(STATE::MOVE);
}

void Neko::Update(void)
{

    // --- 毎フレーム、マウスが壁の上にあるか判定する ---
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = NEKO_WID / 2.0f;
        float halfH = NEKO_HIG / 2.0f;

        isMouseOver_ =
            (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
                mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);
    }

    switch (state_)
    {
    case Neko::STATE::NONE:
        break;
    case Neko::STATE::STANDBY:
        UpdateStandby();
        break;
    case Neko::STATE::MOVE:
        UpdateMove();
        break;
    case Neko::STATE::EAT:
        UpdateEat();
        break;
	case Neko::STATE::PC:
		UpdatePC();
		break;
	case Neko::STATE::TV:
		UpdateTV();
		break;
    case Neko::STATE::ACT:
        UpdateAct();
        break;
    case Neko::STATE::GAMEOVER:
        UpdateGameover();
        break;
    case Neko::STATE::END:
        UpdateEnd();
        break;
    default:
        break;
    }
}

void Neko::Draw(void)
{
    switch (state_)
    {
    case Neko::STATE::NONE:
        break;
    case Neko::STATE::STANDBY:
        DrawStandby();
        break;
    case Neko::STATE::MOVE:
        DrawMove();
        break;
    case Neko::STATE::EAT:
        DrawEat();
        break;
	case Neko::STATE::PC:
		DrawPC();
		break;
	case Neko::STATE::TV:
		DrawTV();
        break;
    case Neko::STATE::ACT:
        DrawAct();
        break;
    case Neko::STATE::GAMEOVER:
        DrawGameover();
        break;
    case Neko::STATE::END:
        DrawEnd();
        break;
    default:
        break;
    }
	

}

void Neko::Release(void)
{
	DeleteGraph(img_);
}

void Neko::SetFood(Food* food)
{
    food_ = food;
}

void Neko::SetPC(PC* pc)
{
    pc_ = pc;
}

void Neko::SetTV(TV* tv)
{
	tv_ = tv;
}

bool Neko::GetIsMouseOver() const
{
    return isMouseOver_;
}

void Neko::Move(void)
{
    // タイマー減少
    if (--moveTimer_ <= 0)
    {
        moveTimer_ = 60 + rand() % 120; // 1〜3秒ごとに行動を決め直す

        if (rand() % 100 < 30)
        {
            // 30%の確率で停止
            moveDirX_ = moveDirY_ = 0;
            isMoving_ = false;
        }
        else
        {
            // 動いていない時だけ方向を新しく決める
            if (!isMoving_)
            {
                float dx = (float)(rand() % 200 - 100) / 100.0f;
                float dy = (float)(rand() % 200 - 100) / 100.0f;
                float len = sqrtf(dx * dx + dy * dy);

                if (len > 0.01f) {
                    moveDirX_ = dx / len;
                    moveDirY_ = dy / len;
                }
                else {
                    moveDirX_ = moveDirY_ = 0;
                }
            }
            isMoving_ = true;
        }
    }

    // 移動処理
    float speed = 1.0f;
    if (isMoving_) {
        pos_.x += moveDirX_ * speed;
        pos_.y += moveDirY_ * speed;
    }

    // 画面下半分の範囲で反射制限
    float lowerLimit = Application::SCREEN_SIZE_Y - NEKO_HIG / 2;
    float upperLimit = Application::SCREEN_SIZE_Y / 2 + NEKO_HIG / 2;

    if (pos_.x < NEKO_WID / 2) { pos_.x = NEKO_WID / 2; moveDirX_ *= -1; }
    if (pos_.x > Application::SCREEN_SIZE_X - NEKO_WID / 2) { pos_.x = Application::SCREEN_SIZE_X - NEKO_WID / 2; moveDirX_ *= -1; }
    if (pos_.y < upperLimit) { pos_.y = upperLimit; moveDirY_ *= -1; }
    if (pos_.y > lowerLimit) { pos_.y = lowerLimit; moveDirY_ *= -1; }

    /*// アニメーション更新
    if (isMoving_) {
        frameCounter_++;
        if (frameCounter_ % 10 == 0) { // 10フレームごとに切り替え
            imgIndex_ = (imgIndex_ + 1) % 4;
        }
    }
    else {
        imgIndex_ = 0;
    }*/
}

void Neko::MoveToTarget(VECTOR targetPos, bool targetFlag)
{
    float dx = targetPos.x - pos_.x;
    float dy = targetPos.y - pos_.y;
    float len = sqrtf(dx * dx + dy * dy);

    if (len > 5.0f)
    {
        float speed = 2.0f;
        pos_.x += dx / len * speed;
        pos_.y += dy / len * speed;
    }
    else
    {
        // 到達
        pos_.x = targetPos.x;
        pos_.y = targetPos.y;
        moveDirX_ = moveDirY_ = 0;
        isMoving_ = false;

        targetFlag = false;
        targetType_ = TARGET::NONE;
        ChangeState(STATE::STANDBY);
    }
}

void Neko::ChangeState(STATE state)
{
    state_ = state;

    switch (state)
    {
    case Neko::STATE::NONE:
        break;
    case Neko::STATE::STANDBY:
        ChangeStandby();
        break;
    case Neko::STATE::MOVE:
        ChangeMove();
        break;
    case Neko::STATE::EAT:
        ChangeEat();
        break;
	case Neko::STATE::PC:
		ChangePC();
		break;
	case Neko::STATE::TV:
		ChangeTV();
		break;
    case Neko::STATE::ACT:
        ChangeAct();
        break;
    case Neko::STATE::GAMEOVER:
        ChangeGameover();
        break;
    case Neko::STATE::END:
        ChangeEnd();
        break;
    default:
        break;
    }
}

void Neko::ChangeStandby(void)
{
    standbyTimer_ = 120; // 2秒待つ
}

void Neko::ChangeMove(void)
{
    
}

void Neko::ChangeEat(void)
{
}

void Neko::ChangePC(void)
{
}

void Neko::ChangeTV(void)
{
}

void Neko::ChangeAct(void)
{
}

void Neko::ChangeGameover(void)
{
}

void Neko::ChangeEnd(void)
{
}

void Neko::UpdateStandby(void)
{
	if (--standbyTimer_ <= 0)
	{
		ChangeState(STATE::MOVE);
		//return;
	}
}

void Neko::UpdateMove(void)
{
    // --- 優先順位: Food > PC > TV ---
    /*if (food_ && food_->GetFlag())
    {
        targetType_ = TARGET::FOOD;
        ChangeState(STATE::EAT);
        return;
    }
    
    else*/ if (tv_ && tv_->GetFlag())
    {
        targetType_ = TARGET::TV;
        ChangeState(STATE::TV);
        return;
	}
	else if (pc_ && pc_->GetFlag())
	{
		targetType_ = TARGET::PC;
		ChangeState(STATE::PC);
		return;
	}

    Move();
}

void Neko::UpdateEat(void)
{
    if (!food_ || !food_->GetFlag())
    {
        ChangeState(STATE::MOVE);
        return;
    }

    MoveToTarget(food_->GetPos(), food_->GetFlag());
}

void Neko::UpdatePC(void)
{
    if (!pc_ || !pc_->GetFlag())
    {
        ChangeState(STATE::MOVE);
        return;
    }

    MoveToTarget(pc_->GetPos(), pc_->GetFlag());
}

void Neko::UpdateTV(void)
{
    if (!tv_ || !tv_->GetFlag())
    {
        ChangeState(STATE::MOVE);
        return;
    }

    MoveToTarget(tv_->GetPos(), tv_->GetFlag());
}

void Neko::UpdateAct(void)
{
}

void Neko::UpdateGameover(void)
{
}

void Neko::UpdateEnd(void)
{
}

void Neko::DrawStandby(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false);
    }
}

void Neko::DrawMove(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false);
    }
}

void Neko::DrawEat(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false);
    }
}

void Neko::DrawPC(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false);
    }
}

void Neko::DrawTV(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false);
    }
}

void Neko::DrawAct(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false);
    }
}

void Neko::DrawGameover(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false);
    }
}

void Neko::DrawEnd(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false);
    }
}
