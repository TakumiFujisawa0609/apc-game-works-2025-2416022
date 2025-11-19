#pragma once

#include "MinigameBase.h"

class TVMinigame : public MinigameBase
{
public:

    static constexpr int GAUGE_WIDTH = 500;
    static constexpr int GAUGE_HEIGHT = 20;
    static constexpr int PERFECT_WIDTH = 30; // Perfectゾーンの幅 (中央15pxずつ)
    static constexpr int MARGIN_X = 50;

    TVMinigame() : MinigameBase(800, 600) {} // TV用の画面サイズを設定

    void Init() override;
    bool UpdateGame() override; // ★ TV独自のミニゲームロジック
    void DrawGame() override;   // ★ TV独自のミニゲーム描画

private:
    
    enum class STATE {
        READY,      // 開始前の待機
        GAUGE_MOVE, // ゲージが移動中
        JUDGED,     // 判定完了
        CLEARED     // クリア状態
    };

    STATE gameState_;

    // ゲージ関連
    float gaugeValue_;      // 現在のゲージ位置 (0.0f ~ 1.0f)
    float gaugeSpeed_;      // ゲージの移動速度 (例: 0.01f/フレーム)
    int gaugeDirection_;    // ゲージの移動方向 (1:右, -1:左)

    // タイミング関連
    int perfectZoneStart_;  // Perfect判定ゾーンの開始位置 (ピクセル)
    int perfectZoneEnd_;    // Perfect判定ゾーンの終了位置 (ピクセル)

    // 描画関連
    int gaugeBarWidth_;     // ゲージバー全体の幅 (ピクセル)
    int gaugeBarHeight_;    // ゲージバー全体の高さ (ピクセル)

};