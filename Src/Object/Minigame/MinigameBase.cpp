
#include "../../Manager/Generic/InputManager.h"
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "MinigameBase.h"

MinigameBase::MinigameBase(int width, int height)
{
}

void MinigameBase::Init()
{
}

bool MinigameBase::UpdateGame()
{
    return false;
}

void MinigameBase::Draw()
{

    // 派生クラス独自のゲーム内容を描画
    DrawGame();
}

// MinigameBase::Start() で drawPos_ を計算する
void MinigameBase::Start()
{
    const int screenW = Application::SCREEN_SIZE_X;
    const int screenH = Application::SCREEN_SIZE_Y;

    // ミニゲームのサイズ
    const int gameW = 800;
    const int gameH = 600;

    // ★修正点2: 計算ロジックを修正★
    // (画面サイズ - ミニゲームサイズ) / 2 で描画開始座標を計算
    drawPos_.x = (float)(screenW / 2 - gameW / 2);
    drawPos_.y = (float)(screenH / 2 - gameH / 2);
}

void MinigameBase::End()
{
}