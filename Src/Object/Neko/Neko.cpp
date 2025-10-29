
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "Neko.h"

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

    ChangeState(STATE::MOVE);
}

void Neko::Update(void)
{
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
    // --- Foodが有効なら、EAT状態に移行 ---
    if (food_ && food_->GetFlag())
    {
        ChangeState(STATE::EAT);
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

    const VECTOR& foodPos = food_->GetPos();
    float dx = foodPos.x - pos_.x;
    float dy = foodPos.y - pos_.y;
    float len = sqrtf(dx * dx + dy * dy);

    if (len > 5.0f) // ← 少し余裕を持たせる
    {
        float speed = 1.5f;
        pos_.x += dx / len * speed;
        pos_.y += dy / len * speed;
    }
    else
    {
        // --- 食べ物に到達 ---
        pos_.x = foodPos.x;
        pos_.y = foodPos.y;
        moveDirX_ = 0.0f;
        moveDirY_ = 0.0f;
        isMoving_ = false;

        food_->SetFlag(false);
        ChangeState(STATE::STANDBY);
    }
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

    DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
        pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
        GetColor(255, 0, 0), false);
}

void Neko::DrawMove(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
        pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
        GetColor(255, 0, 0), false);
}

void Neko::DrawEat(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
        pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
        GetColor(255, 0, 0), false);
}

void Neko::DrawAct(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
        pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
        GetColor(255, 0, 0), false);
}

void Neko::DrawGameover(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
        pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
        GetColor(255, 0, 0), false);
}

void Neko::DrawEnd(void)
{
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
        pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
        GetColor(255, 0, 0), false);
}
