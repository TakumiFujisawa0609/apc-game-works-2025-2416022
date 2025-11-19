#include "TVMinigame.h"
#include <DxLib.h>
#include <cmath>

void TVMinigame::Init()
{

    gaugeBarWidth_ = GAUGE_WIDTH;
    gaugeBarHeight_ = GAUGE_HEIGHT;

    // Perfectゾーンの位置設定 (中央から±15px)
    int center = GAUGE_WIDTH / 2;
    perfectZoneStart_ = center - PERFECT_WIDTH / 2;
    perfectZoneEnd_ = center + PERFECT_WIDTH / 2;

    // ゲームの状態を初期化
    gameState_ = STATE::READY;
    gaugeValue_ = 0.0f; // ゲージは左端からスタート
    gaugeSpeed_ = 0.015f; // 速度
    gaugeDirection_ = 1; // 右へ移動
    isCleared_ = false;
}

bool TVMinigame::UpdateGame()
{
    // マウス左クリック入力をチェック
    bool isClicked = (GetMouseInput() & MOUSE_INPUT_LEFT);

    switch (gameState_)
    {
    case STATE::READY:
        // 最初のクリックでゲージをスタート
        if (isClicked) {
            gameState_ = STATE::GAUGE_MOVE;
        }
        break;

    case STATE::GAUGE_MOVE:
        // ゲージを移動させる
        gaugeValue_ += gaugeSpeed_ * gaugeDirection_;

        // 端に達したら移動方向を反転 (跳ね返り)
        if (gaugeValue_ >= 1.0f) {
            gaugeValue_ = 1.0f;
            gaugeDirection_ = -1;
        }
        else if (gaugeValue_ <= 0.0f) {
            gaugeValue_ = 0.0f;
            gaugeDirection_ = 1;
        }

        // クリックされたら判定へ
        if (isClicked) {
            gameState_ = STATE::JUDGED;
        }
        break;

    case STATE::JUDGED:
    {// 判定処理を実行 (このフレームで判定は終わったと見なす)

        // 現在のゲージのピクセル位置を計算
        int currentGaugeX = (int)(gaugeValue_ * gaugeBarWidth_);

        if (currentGaugeX >= perfectZoneStart_ && currentGaugeX <= perfectZoneEnd_)
        {
            // Perfectゾーン内ならクリア
            isCleared_ = true;
        }
        else
        {
            // 失敗
            isCleared_ = false;
        }

        // 判定が終わったら、結果を数フレーム表示するためにSTATE::CLEAREDへ移行
        gameState_ = STATE::CLEARED;

        return isCleared_;
    }
        // 成功/失敗の結果表示タイマーなどを設定しても良いが、ここでは単純に終了フラグを返す
        break;

    case STATE::CLEARED:
        // 判定結果表示中... (ここでは即座に結果を返すと想定)
        return isCleared_;

    default:
        break;
    }

    // ゲーム続行中は false を返す
    return false;
}

void TVMinigame::DrawGame()
{
    // 描画位置の基準点 (親クラスのdrawPos_を使用)
    int baseX = (int)drawPos_.x + MARGIN_X;
    int baseY = (int)drawPos_.y + 100;

    // --- 1. ゲージの背景とゾーンの描画 ---

    // ゲージの背景（灰色）
    DrawBox(baseX, baseY,
        baseX + gaugeBarWidth_, baseY + gaugeBarHeight_,
        GetColor(50, 50, 50), TRUE);

    // Perfectゾーン（緑色）
    DrawBox(baseX + perfectZoneStart_, baseY,
        baseX + perfectZoneEnd_, baseY + gaugeBarHeight_,
        GetColor(0, 255, 0), TRUE);

    // ゲージの外枠
    DrawBox(baseX, baseY,
        baseX + gaugeBarWidth_, baseY + gaugeBarHeight_,
        GetColor(255, 255, 255), FALSE);


    // --- 2. 移動するゲージの描画 ---
    if (gameState_ != STATE::READY)
    {
        int gaugeX = baseX + (int)(gaugeValue_ * gaugeBarWidth_);
        int gaugeRadius = gaugeBarHeight_ / 2 + 5; // 見やすいように少し大きく

        // 移動するインジケータ（赤色の円など）
        DrawCircle(gaugeX, baseY + gaugeBarHeight_ / 2, gaugeRadius, GetColor(255, 50, 50), TRUE);
    }

    // --- 3. 状態テキストの描画 ---
    const char* message = "";
    int color = GetColor(255, 255, 255);

    switch (gameState_)
    {
    case STATE::READY:
        message = "Click to Start!";
        break;
    case STATE::GAUGE_MOVE:
        message = "Click NOW!";
        break;
    case STATE::JUDGED:
        message = "JUDGING...";
        break;
    case STATE::CLEARED:
        message = isCleared_ ? "PERFECT!" : "FAILED...";
        color = isCleared_ ? GetColor(255, 255, 0) : GetColor(255, 0, 0);
        break;
    }

    DrawString(baseX, baseY - 40, message, color);
}