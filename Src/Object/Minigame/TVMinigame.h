#pragma once

#include "MinigameBase.h"

class TVMinigame : public MinigameBase
{
public:
    TVMinigame() : MinigameBase(800, 400) {} // TV用の画面サイズを設定

    void Init() override;
    bool UpdateGame() override; // ★ TV独自のミニゲームロジック
    void DrawGame() override;   // ★ TV独自のミニゲーム描画

private:
    // TVミニゲーム独自のメンバー変数
    int hackProgress_;
    // ...
};