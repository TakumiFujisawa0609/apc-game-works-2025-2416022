#pragma once

#include "MinigameBase.h"

class PCMinigame : public MinigameBase
{
public:
    PCMinigame() : MinigameBase(800, 400) {} // PC用の画面サイズを設定

    void Init() override;
    bool UpdateGame() override; // ★ PC独自のミニゲームロジック
    void DrawGame() override;   // ★ PC独自のミニゲーム描画

private:
    // PCミニゲーム独自のメンバー変数
    int hackProgress_;
    // ...
};