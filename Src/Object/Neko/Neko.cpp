
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
	isMoving_ = true;
	isMouseOver_ = false;
	isVisible_ = true;
	justHidden_ = false;
	targetType_ = TARGET::NONE;

    ChangeState(STATE::MOVE);
}

void Neko::Update()
{

    // --- 毎フレーム、マウスがネコの上にあるか判定 ---
    Vector2 mousePos = InputManager::GetInstance().GetMousePos();
    float halfW = NEKO_WID / 2.0f;
    float halfH = NEKO_HIG / 2.0f;

    isMouseOver_ =
        (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
            mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);

    // ネコ非表示でターゲットがあれば再表示
    if (!isVisible_ && ((food_ && food_->GetFlag()) || (pc_ && pc_->GetFlag()) || (tv_ && tv_->GetFlag())))
    {
        isVisible_ = true;
        justHidden_ = false;

        // ターゲット優先で状態変更
        if (food_ && food_->GetFlag()) ChangeState(STATE::EAT);
        else if (pc_ && pc_->GetFlag()) ChangeState(STATE::PC);
        else if (tv_ && tv_->GetFlag()) ChangeState(STATE::TV);
    }

    // ネコ非表示でターゲットがない場合は処理終了
    if (!isVisible_) return;

    // --- 状態別更新 ---
    switch (state_)
    {
    case STATE::STANDBY: 
        UpdateStandby();
        break;
    case STATE::MOVE:    
        UpdateMove();    
        break;
    case STATE::EAT:     
        UpdateEat();     
        break;
    case STATE::PC:      
        UpdatePC();      
        break;
    case STATE::TV:     
        UpdateTV();      
        break;
    case STATE::ACT:     
        UpdateAct();     
        break;
    case STATE::GAMEOVER:
        UpdateGameover(); 
        break;
    case STATE::END:     
        UpdateEnd();     
        break;
    default: 
        break;
    }
}


void Neko::Draw(void)
{
	if (!isVisible_) return;

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

void Neko::SelectTarget()
{
    // 各ターゲットが有効かどうかチェック
    bool foodValid = (food_ && food_->GetFlag());
    bool pcValid = (pc_ && pc_->GetFlag());
    bool tvValid = (tv_ && tv_->GetFlag());

    // 優先度の高い順にチェック（Food > PC > TV）
    if (foodValid)
    {
        targetType_ = TARGET::FOOD;
        ChangeState(STATE::EAT);
    }
    else if (pcValid)
    {
        targetType_ = TARGET::PC;
        ChangeState(STATE::PC);
    }
    else if (tvValid)
    {
        targetType_ = TARGET::TV;
        ChangeState(STATE::TV);
    }
    else
    {
        targetType_ = TARGET::NONE;
        if (state_ != STATE::MOVE)
            ChangeState(STATE::MOVE);
    }
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
                    isMoving_ = true;
                }
                else {
                    moveDirX_ = moveDirY_ = 0;
                    isMoving_ = false;
                }
            }
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

    if (len > 0.01f) {
        moveDirX_ = dx / len;
        moveDirY_ = dy / len;
        isMoving_ = true;
    }
    else {
        moveDirX_ = moveDirY_ = 0;
        isMoving_ = false;
    }

    float speed = 2.0f; // 追跡は少し速く
    pos_.x += moveDirX_ * speed;
    pos_.y += moveDirY_ * speed;

    if (len < 5.0f) // 到達判定
    {
        // --- 対象ごとのリアクション ---
        switch (targetType_)
        {
        case TARGET::TV:
            if (tv_) {
                tv_->SetFlag(false);
                //tv_->ChangeImage(); // TVの画像を変更
            }
            break;

        case TARGET::FOOD:
            if (food_) {
                food_->SetFlag(false);
                //food_->ChangeImage(); // Foodの画像を変更
            }
            break;

        case TARGET::PC:
            if (pc_) {
                pc_->SetFlag(false);
                //pc_->ChangeImage(); // PCの画像を変更
            }
            break;
        }

        // --- ネコを非表示にする ---
        /*isVisible_ = false;
		justHidden_ = true;*/

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

void Neko::UpdateMove()
{
    // ターゲットが存在すれば状態変更する
    if ((food_ && food_->GetFlag()) ||
        (pc_ && pc_->GetFlag()) ||
        (tv_ && tv_->GetFlag()))
    {
        SelectTarget();
        return;
    }

    // ターゲットがなければランダム移動
    Move();
}

void Neko::UpdateEat()
{
    if (!food_ || !food_->GetFlag())
    {
        ChangeState(STATE::MOVE);
        targetType_ = TARGET::NONE;
        return;
    }

    MoveToTarget(food_->GetPos(), true);
}

void Neko::UpdatePC()
{
    // 優先度の高いターゲットを毎フレーム確認
    if (food_ && food_->GetFlag())
    {
        // Food が最優先なので状態を切り替える
        SelectTarget();
        return;
    }

    if (!pc_ || !pc_->GetFlag())
    {
        ChangeState(STATE::MOVE);
        targetType_ = TARGET::NONE;
        return;
    }

    MoveToTarget(pc_->GetTargetPos(), true);
}

void Neko::UpdateTV()
{
    // 優先度の高いターゲットを毎フレーム確認
    if (food_ && food_->GetFlag())
    {
        // Food が最優先なので状態を切り替える
        SelectTarget();
        return;
    }

    if (!tv_ || !tv_->GetFlag())
    {
        ChangeState(STATE::MOVE);
        targetType_ = TARGET::NONE;
        return;
    }

    MoveToTarget(tv_->GetPos(), true);
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
