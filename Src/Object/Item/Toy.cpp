#include "Toy.h"
#include "Food.h"
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include <DxLib.h>
#include <cmath>

Toy::Toy(void)
{
}

Toy::~Toy(void)
{
}

void Toy::Init(void)
{
    // 画像読み込み
    img_ = LoadGraph((Application::PATH_ITEM + "nc279907.png").c_str());
    img2_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ.png").c_str());
    shadowImg_ = LoadGraph((Application::PATH_ITEM + "nc305932.png").c_str());

    imgA_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ1.png").c_str());
    imgB_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ2.png").c_str());
    imgC_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ3.png").c_str());
    imgD_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ4.png").c_str());
    imgE_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ5.png").c_str());
    imgF_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ6.png").c_str());
    imgG_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ7.png").c_str());
    imgH_ = LoadGraph((Application::PATH_ITEM + "ぬいぐるみ8.png").c_str());


    pos_.x = 900;
    pos_.y = 650;

    flagImg_ = false;
    isMouseOver_ = false;
    flag_ = false;

    spawnTimerBase_ = 300;
    spawnTimerMultiplier_ = 1.0f;
    spawnInterval_ = 300;
    spawnTimer_ = 180 + rand() % 300;
    count_ = 0;
    returnTimer_ = 0;

    fallY_ = pos_.y;
    fallSpeed_ = 0.0f;
    isFalling_ = false;

    flagShadow_ = false;
    shadowTimer_ = 0;
    shadowAlpha_ = 0.0f;

    // Food連携用の初期化
    state_ = State::WAITING;
    targetPos_ = VGet(0, 0, 0);
    moveSpeed_ = 5.0f;  // 移動速度
    eatingTimer_ = 0;
    shakeOffset_ = 0.0f;
    shakeCount_ = 0;
}

void Toy::Update(Food* food)
{
    // ゲームオーバー状態の場合は処理を終了
    if (state_ == State::GAME_OVER)
    {
        UpdateGameOver();
        return;
    }

    // --- 毎フレーム、マウスが上にあるか判定する ---
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = TOY_WID / 2.0f;
        float halfH = TOY_HIG / 2.0f;

        isMouseOver_ =
            (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
                mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);
    }

    // --- 状態別の処理 ---
    switch (state_)
    {
    case State::WAITING:
        // --- クリック処理 ---
        if (flagImg_ && InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_)
        {
            // おもちゃをクリックした → 非表示にして再出現タイマーをリセット
            flagImg_ = false;
            isFalling_ = false;

            pos_.x = 900;
            pos_.y = 650;
            fallY_ = pos_.y;

            spawnTimer_ = 180 + rand() % 300;
            spawnTimerMultiplier_ = 1.0f;

            ResetShadow();
            count_ = 0;
            return;
        }

        // --- 再出現タイマー処理 ---
        if (!flagImg_)
        {
            spawnTimer_--;

            // 出現3秒前から影をフェードイン
           const int SHADOW_FADE_START = 180;
            if (spawnTimer_ <= SHADOW_FADE_START && spawnTimer_ > 0)
            {
                flagShadow_ = true;
                shadowAlpha_ += 2.0f;
                if (shadowAlpha_ > 255.0f) shadowAlpha_ = 255.0f;
            }

            // --- 出現タイマー終了 → Toy出現＋落下開始 ---
            if (spawnTimer_ <= 0)
            {
                flagImg_ = true;
                isFalling_ = true;
                fallY_ = -100;
                fallSpeed_ = 0.0f;

                flagShadow_ = true;
                if (shadowAlpha_ < 50.0f) shadowAlpha_ = 50.0f;
            }
        }

        // --- 落下演出 ---
        if (isFalling_)
        {
            fallSpeed_ += 1.5f;
            fallY_ += fallSpeed_;

            shadowAlpha_ += 3.0f;
            if (shadowAlpha_ > 180.0f) shadowAlpha_ = 180.0f;

            if (fallY_ >= pos_.y)
            {
                fallY_ = pos_.y;
                isFalling_ = false;
                shadowAlpha_ = 180.0f;
            }
        }

        // 落下完了後
        if (!isFalling_ && flagImg_)
        {
            shadowAlpha_ = 180.0f;
            flagShadow_ = true;
        }

        // --- Foodのフラグが立っているか確認 ---
        if (food != nullptr && food->GetFlag() && flagImg_ && !isFalling_)
        {
            // Foodに向かって移動開始
            state_ = State::MOVING;
            targetPos_ = food->GetPos();
        }
        break;

    case State::MOVING:
    case State::EATING: // MOVINGとEATINGの両方で Food が消えたら戻るチェック
    {
        // ★ 修正/追加: Foodがクリックなどで無効になったら、元の位置に戻す
        if (food == nullptr || !food->GetFlag())
        {
            // 状態を RETURNING に変更
            state_ = State::RETURNING;

            // 止まる
            targetPos_ = pos_;

            // 立ち止まる時間（例：3秒 = 180フレーム）を設定
            returnTimer_ = 180;

            // 落下状態やタイマーもリセット（必要に応じて）
            //isFalling_ = false;
            eatingTimer_ = 0;
            shakeCount_ = 0;
            return; // ここで処理を終える
        }

        // MOVING 状態の処理
        if (state_ == State::MOVING) {
            UpdateMoving();

            // Foodに到達したか確認
            float dx = targetPos_.x - pos_.x;
            float dy = targetPos_.y - pos_.y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < 10.0f)
            {
                // 到達したので食事状態へ
                state_ = State::EATING;
                eatingTimer_ = 0;
                shakeCount_ = 0;
            }
        }

        // EATING 状態の処理
        else if (state_ == State::EATING) {
            UpdateEating();
        }

    }
        break;

    case State::RETURNING:
    {
        // 立ち止まる
        if (returnTimer_ > 0)
        {
            // タイマーが残っている間は、その場で静止
            returnTimer_--;
        }
        else
        {
            // ★ 修正点: タイマーがゼロになったら、その場から動かずに WAITING 状態へ戻る
            //           (座標の瞬間移動やリセットは行わない)
            state_ = State::WAITING;
        }
    }
    break;

    case State::GAME_OVER:
        UpdateGameOver();
        break;
    }
}

void Toy::UpdateMoving()
{
    // Foodに向かって移動
    float dx = targetPos_.x - pos_.x;
    float dy = targetPos_.y - pos_.y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance > 0.1f)
    {
        // 正規化して移動
        float nx = dx / distance;
        float ny = dy / distance;

        pos_.x += nx * moveSpeed_;
        pos_.y += ny * moveSpeed_;
        fallY_ = pos_.y; // 描画位置も更新
    }
}

void Toy::UpdateEating()
{
    eatingTimer_++;

    // 上下に細かく震える演出
    shakeCount_++;
    shakeOffset_ = sin(shakeCount_ * 1.0f) * 3.0f; // 振幅3ピクセル

    // 制限時間を超えたらゲームオーバー
    if (eatingTimer_ >= EATING_LIMIT)
    {
        state_ = State::GAME_OVER;
    }
}

void Toy::UpdateGameOver()
{
    // ゲームオーバー演出（必要に応じて実装）
    // 例：点滅、色変更、拡大など
}

void Toy::Draw(void)
{
    float halfW = TOY_WID / 2.0f;
    float halfH = TOY_HIG / 2.0f;

    // 影の描画
    DrawShadow();

    // おもちゃ本体の描画
    if (flagImg_ || state_ == State::MOVING || state_ == State::EATING || state_ == State::GAME_OVER)
    {
        float drawY = fallY_;

        // 食事中は震え演出を追加
        if (state_ == State::EATING)
        {
            drawY += shakeOffset_;
        }

        DrawRotaGraph((int)pos_.x, (int)drawY, 1.0, 0.0, img_, true,true);

        // デバッグ用：当たり判定の可視化
#ifdef _DEBUG
        DrawBox(
            (int)(pos_.x - halfW), (int)(drawY - halfH),
            (int)(pos_.x + halfW), (int)(drawY + halfH),
            GetColor(0, 0, 255), false);

        // 状態表示
        const char* stateStr = "";
        switch (state_)
        {
        case State::WAITING: stateStr = "WAITING"; break;
        case State::MOVING: stateStr = "MOVING"; break;
        case State::EATING: stateStr = "EATING"; break;
        case State::GAME_OVER: stateStr = "GAME_OVER"; break;
        }
        DrawFormatString((int)pos_.x - 30, (int)pos_.y - 80, GetColor(255, 255, 255),
            "State: %s", stateStr);

        if (state_ == State::EATING)
        {
            DrawFormatString((int)pos_.x - 30, (int)pos_.y - 60, GetColor(255, 255, 0),
                "Time: %d/%d", eatingTimer_, EATING_LIMIT);
        }
#endif
    }

    // マウスオーバー時の枠線表示（ゲーム機能）
    if (isMouseOver_ && state_ == State::WAITING) {
        DrawBox(
            (int)(pos_.x - halfW), (int)(pos_.y - halfH),
            (int)(pos_.x + halfW), (int)(pos_.y + halfH),
            GetColor(255, 0, 0), false);
    }

    // ゲームオーバー時の表示
    if (state_ == State::GAME_OVER)
    {
        DeleteGraph(img_);
    }
}

void Toy::Release(void)
{
    DeleteGraph(img_);
    DeleteGraph(shadowImg_);
}

VECTOR Toy::GetPos(void) const
{
    return pos_;
}

bool Toy::GetFlag(void) const
{
    return flagImg_;
}

bool Toy::GetFlagShadow(void) const
{
    return flagShadow_;
}

bool Toy::GetIsMouseOver() const
{
    return isMouseOver_;
}

void Toy::DrawShadow()
{
    if (flagShadow_ || state_ == State::MOVING || state_ == State::EATING)
    {
        int alpha = (int)shadowAlpha_;
        if (alpha > 255) alpha = 255;
        if (alpha < 0) alpha = 0;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawRotaGraph((int)pos_.x, (int)(pos_.y + 20), 0.1, 0.0, shadowImg_, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void Toy::ResetShadow()
{
    flagShadow_ = false;
    shadowAlpha_ = 0.0f;
}