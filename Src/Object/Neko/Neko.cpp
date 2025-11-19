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
    img_ = LoadGraph((Application::PATH_MODEL + "Nekoproto.jpg").c_str());

    pos_.x = Application::SCREEN_SIZE_X / 2;
    pos_.y = Application::SCREEN_SIZE_Y / 2 + 100;

    moveTimer_ = 0;
    isMoving_ = false;
    isMouseOver_ = false;
    isVisible_ = true;
    justHidden_ = false;
    justChangedState_ = false;
    targetType_ = TARGET::NONE;
    targetTimer_ = 0;

    ChangeState(STATE::STANDBY);
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
        SelectTarget();
    }

    // ネコ非表示でターゲットがない場合は処理終了
    if (!isVisible_) return;

    if (targetCoolDown_ > 0) {
        targetCoolDown_--;
    }

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

bool Neko::ShouldSwitchTarget()
{
    // Foodが最優先
    if (food_ && food_->GetFlag() && targetType_ != TARGET::FOOD) {
        return true;
    }

    // クールダウンチェック
    bool isPCCoolDown = (lastTargetType_ == TARGET::PC && targetCoolDown_ > 0);
    bool isTVCoolDown = (lastTargetType_ == TARGET::TV && targetCoolDown_ > 0);

    // PCが現在のターゲットであり、TVが有効でクールダウン中でないなら、切り替えを許可
    if (targetType_ == TARGET::PC && tv_ && tv_->GetFlag() && !isTVCoolDown) {
        return true;
    }

    // TVが現在のターゲットであり、PCが有効でクールダウン中でないなら、切り替えを許可
    if (targetType_ == TARGET::TV && pc_ && pc_->GetFlag() && !isPCCoolDown) {
        return true;
    }

    return false;
}

void Neko::SelectTarget()
{
    bool foodValid = (food_ && food_->GetFlag());
    bool pcFlagValid = (pc_ && pc_->GetFlag());
    bool tvFlagValid = (tv_ && tv_->GetFlag());

    // クールダウンチェックをターゲット選択に追加
    bool isPCCoolDown = (lastTargetType_ == TARGET::PC && targetCoolDown_ > 0);
    bool isTVCoolDown = (lastTargetType_ == TARGET::TV && targetCoolDown_ > 0);

    bool pcValid = pcFlagValid && !isPCCoolDown; // クールダウン中は無効
    bool tvValid = tvFlagValid && !isTVCoolDown; // クールダウン中は無効

    // すでにターゲットがある場合の上書きルール
    if (targetType_ != TARGET::NONE)
    {
        // 1. Food が出現した場合は最優先で上書き
        if (foodValid && targetType_ != TARGET::FOOD)
        {
            targetType_ = TARGET::FOOD;
            ChangeState(STATE::EAT);
            return;
        }

        // 2. PCにいるときにTVが有効/TVにいるときにPCが有効なら上書き
        // ShouldSwitchTarget() が true を返している前提
        /*if (targetType_ == TARGET::PC && tvValid)
        {
            targetType_ = TARGET::TV;
            ChangeState(STATE::TV);
            return;
        }
        if (targetType_ == TARGET::TV && pcValid)
        {
            targetType_ = TARGET::PC;
            ChangeState(STATE::PC);
            return;
        }*/

        // 3. 現在ターゲットが無効になったら再選択
        else if (
            (targetType_ == TARGET::FOOD && !foodValid) ||
            (targetType_ == TARGET::PC && !pcValid) || // pcValid を使用
            (targetType_ == TARGET::TV && !tvValid))   // tvValid を使用
        {
            targetType_ = TARGET::NONE;
            SelectTarget(); // 再帰的に再選択
            return;
        }

        // 現在のターゲットが有効ならそのまま継続
        return;
    }

    // --- 新規ターゲット選択 (targetType_ == TARGET::NONE) ---
    if (foodValid)
    {
        targetType_ = TARGET::FOOD;
        ChangeState(STATE::EAT);
    }
    else
    {
        TARGET nextTarget = TARGET::NONE;

        if (lastTargetType_ == TARGET::PC)
        {
            // PC滞在後: TVを優先してチェック
            if (tvValid) { nextTarget = TARGET::TV; }
            else if (pcValid) { nextTarget = TARGET::PC; }
        }
        else if (lastTargetType_ == TARGET::TV)
        {
            // TV滞在後: PCを優先してチェック
            if (pcValid) { nextTarget = TARGET::PC; }
            else if (tvValid) { nextTarget = TARGET::TV; }
        }
        else // lastTargetType_ が NONE, FOOD, または初期状態の場合（デフォルトの優先順位）
        {
            // デフォルト: PCを優先してチェック
            if (pcValid) { nextTarget = TARGET::PC; }
            else if (tvValid) { nextTarget = TARGET::TV; }
        }

        if (nextTarget == TARGET::PC)
        {
            targetType_ = TARGET::PC;
            ChangeState(STATE::PC);
        }
        else if (nextTarget == TARGET::TV)
        {
            targetType_ = TARGET::TV;
            ChangeState(STATE::TV);
        }
        else
        {
            targetType_ = TARGET::NONE;
            if (state_ != STATE::STANDBY)
                ChangeState(STATE::STANDBY);
        }
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
}

void Neko::MoveToTarget(VECTOR targetPos, bool targetFlag)
{
    float dx = targetPos.x - pos_.x;
    float dy = targetPos.y - pos_.y;
    float len = sqrtf(dx * dx + dy * dy);

    // 到達判定
    const float ARRIVAL_DISTANCE = 2.0f;
    if (len < ARRIVAL_DISTANCE) {
        OnArriveTarget();  // 到達時の処理
        return;
    }

    // 移動処理
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
}

void Neko::OnArriveTarget()
{
    // ネコの座標をターゲット位置に固定する
    VECTOR targetPos = pos_;

    switch (targetType_)
    {
    case TARGET::FOOD:
        if (food_) {
            food_->ChangeImage();
            targetPos = food_->GetPos();
        }
        pos_ = targetPos;
        targetType_ = TARGET::NONE;
        ChangeState(STATE::STANDBY);
        break;

    case TARGET::PC:
        if (pc_) {
            pc_->ChangeImage();
            targetPos = pc_->GetTargetPos();
            pc_->SetMinigameActive(true);  // ミニゲーム有効化
            pos_ = targetPos;

            // 到着後の待機タイマー設定（3秒）
            targetTimer_ = 180;
            // ターゲットは維持したまま（STATE::PCのまま）
        }
        break;

    case TARGET::TV:
        if (tv_) {
            tv_->ChangeImage();
            targetPos = tv_->GetTargetPos();
            tv_->SetMinigameActive(true);  // ミニゲーム有効化
            pos_ = targetPos;

            // 到着後の待機タイマー設定（3秒）
            targetTimer_ = 180;
            // ターゲットは維持したまま（STATE::TVのまま）
        }
        break;
    }

    // 座標を固定
    pos_ = targetPos;
    isMoving_ = false;
    moveDirX_ = 0.0f;
    moveDirY_ = 0.0f;
}

void Neko::ChangeState(STATE state)
{
    state_ = state;
    justChangedState_ = true;

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
    isMoving_ = true;
    moveDirX_ = 0.0f; // 移動方向はMoveToTargetで再計算される
    moveDirY_ = 0.0f;
}

void Neko::ChangeTV(void)
{
    isMoving_ = true;
    moveDirX_ = 0.0f; // 移動方向はMoveToTargetで再計算される
    moveDirY_ = 0.0f;
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

    if (standbyTimer_ > 0)
    {
        --standbyTimer_;
        return; // 待機中は何もしない
    }

    // 新しいターゲットが出現したら反応する
    if ((food_ && food_->GetFlag()) ||
        (pc_ && pc_->GetFlag()) ||
        (tv_ && tv_->GetFlag()))
    {
        SelectTarget();
        return;
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
    //Move();
}

void Neko::UpdateEat()
{
    // 優先度チェック
    if (ShouldSwitchTarget()) {
        SelectTarget();
        return;
    }

    // ターゲットが無効になったら移動状態へ
    if (!food_ || !food_->GetFlag())
    {
        targetType_ = TARGET::NONE;
        ChangeState(STATE::STANDBY);
        return;
    }

    MoveToTarget(food_->GetPos(), true);
}

void Neko::UpdatePC()
{
    // ターゲットが無効になったら待機状態へ
    if (!pc_ || !pc_->GetFlag())
    {
        targetTimer_ = 0;
        targetType_ = TARGET::NONE;
        ChangeState(STATE::STANDBY);
        return;
    }

    // Foodは最優先で反応
    if (food_ && food_->GetFlag() && targetType_ != TARGET::FOOD) {
        targetTimer_ = 0;  // タイマーをリセット
        SelectTarget();
        return;
    }

    // 到着後の待機タイマー処理
    if (targetTimer_ > 0)
    {
        targetTimer_--;
        if (targetTimer_ <= 0)
        {
            // 待機時間終了: 次の行動へ
            targetType_ = TARGET::NONE;
            lastTargetType_ = TARGET::PC;
            targetCoolDown_ = 120;
            standbyTimer_ = 0;
            ChangeState(STATE::STANDBY);
            return;
        }

        

        // 待機中はその場に留まる
        return;
    }

    // 状態遷移直後はターゲット切り替えをスキップ
    /*if (justChangedState_) {
        justChangedState_ = false;
    }
    else {
        // 優先度チェック（移動中のみ実行）
        if (ShouldSwitchTarget()) {
            SelectTarget();
            return;
        }
    }*/

    // ターゲットへ移動
    isMoving_ = true;
    MoveToTarget(pc_->GetTargetPos(), true);
}

void Neko::UpdateTV()
{
    // ターゲットが無効になったら待機状態へ
    if (!tv_ || !tv_->GetFlag())
    {
        targetTimer_ = 0;
        targetType_ = TARGET::NONE;
        ChangeState(STATE::STANDBY);
        return;
    }

    // 待機中でもFoodは最優先で反応
    if (food_ && food_->GetFlag() && targetType_ != TARGET::FOOD) {
        targetTimer_ = 0;  // タイマーをリセット
        SelectTarget();
        return;
    }

    // 到着後の待機タイマー処理
    if (targetTimer_ > 0)
    {
        targetTimer_--;
        if (targetTimer_ <= 0)
        {
            // 待機時間終了: 次の行動へ
            targetType_ = TARGET::NONE;
            lastTargetType_ = TARGET::TV;
            targetCoolDown_ = 120;
            standbyTimer_ = 0;
            ChangeState(STATE::STANDBY);
            return;
        }
        // 待機中はその場に留まる
        return;
    }

    // 状態遷移直後はターゲット切り替えをスキップ
    /*if (justChangedState_) {
        justChangedState_ = false;
    }
    else {
        // 優先度チェック（移動中のみ実行）
        if (ShouldSwitchTarget()) {
            SelectTarget();
            return;
        }
    }*/

    // ターゲットへ移動
    isMoving_ = true;
    MoveToTarget(tv_->GetTargetPos(), true);
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

// ========================================
// 共通描画処理
// ========================================
void Neko::DrawCommon()
{
    bool isReverse = false;
    if (isMoving_) // 移動中のみ方向で判断
    {
        if (moveDirX_ < 0) // 左向きの場合
        {
            isReverse = true;
        }
    }
    else // 静止中の場合
    {
        // 静止中のデフォルト向き（例: 右向き）
        // または、最後に移動した方向を記憶する変数を使う
        // 今回はデフォルトで右向き（反転なし）とする
        isReverse = false;
    }

    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true, isReverse);

    // デバッグ用: マウスオーバー時に枠を表示
    if (isMouseOver_) {
        DrawBox(
            pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
            pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
            GetColor(255, 0, 0), false
        );
    }
}

// ========================================
// 状態別描画処理（共通処理を呼ぶ）
// ========================================
void Neko::DrawStandby(void)
{
    DrawCommon();
}

void Neko::DrawMove(void)
{
    DrawCommon();
}

void Neko::DrawEat(void)
{
    DrawCommon();
}

void Neko::DrawPC(void)
{
    DrawCommon();
}

void Neko::DrawTV(void)
{
    DrawCommon();
}

void Neko::DrawAct(void)
{
    DrawCommon();
}

void Neko::DrawGameover(void)
{
    DrawCommon();
}

void Neko::DrawEnd(void)
{
    DrawCommon();
}