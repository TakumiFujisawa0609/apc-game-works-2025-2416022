#include "PC.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Application.h"
#include "../Minigame/PCMinigame.h"
#include <DxLib.h>
#include <cstdlib>

PC::PC()
// ItemBaseの初期化は自動で行われる
    : ItemBase(5), flagImg_(false), closeW_(170), closeH_(40),
    img3_(0), infoImg_(0)
{
    // ItemBaseのメンバーをPCに合わせて初期化
    spawnInterval_ = 900;

    // ミニゲームインスタンスの生成
    pcMinigame_ = new PCMinigame();
}

PC::~PC()
{
    if (pcMinigame_) {
        delete pcMinigame_;
        pcMinigame_ = nullptr;
    }
}

void PC::Init()
{
    // ItemBaseのメンバーを初期化
    flag_ = false;
    isMouseOver_ = false;
    isGameOver_ = false;
    flagLevel_ = 0;
    progressTimer_ = 0;

    // 画像ロード (img_とimg2_はItemBaseのメンバー)
    img_ = LoadGraph((Application::PATH_ITEM + "desk.png").c_str()); // PC本体
    img2_ = LoadGraph((Application::PATH_ITEM + "nc316039.png").c_str()); // PC画面
    img3_ = LoadGraph((Application::PATH_ITEM + "NyanCat.png").c_str()); // 実績画像
    imgA_ = LoadGraph((Application::PATH_ITEM + ".png").c_str());
    imgB_ = LoadGraph((Application::PATH_ITEM + ".png").c_str());
    imgC_ = LoadGraph((Application::PATH_ITEM + ".png").c_str());
    imgD_ = LoadGraph((Application::PATH_ITEM + ".png").c_str());
    imgE_ = LoadGraph((Application::PATH_ITEM + ".png").c_str());
    infoImg_ = LoadGraph((Application::PATH_STAGE + "黒背景.png").c_str()); // 実績背景
    infoText_ = "実績情報なし";

    pos_ = { 85,500 };
    closePos_ = { 900, 700 };
    cancelPos_ = { 0, 0 };
    cancelW_ = cancelH_ = 0;

    spawnTimer_ = spawnInterval_ + rand() % 301;
}

void PC::Update()
{
    // --- マウスオーバー判定 ---
    checkMouseOver(PC_WID / 2.0f, PC_HIG / 2.0f);

    // --- ランダムでアクティブに ---
    // 実績ウィンドウが開いていない & アクティブでない & ミニゲーム中でない場合
    if (!flagImg_ && !flag_) {
        spawnTimer_--;
        if (spawnTimer_ <= 0) {
            flag_ = true;
            spawnTimer_ = spawnInterval_ + rand() % 301;
        }
    }

    // --- 1. 実績ウィンドウ処理 ---
    if (flagImg_) {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        bool isCloseOver =
            (mousePos.x >= closePos_.x - closeW_ / 2 && mousePos.x <= closePos_.x + closeW_ / 2 &&
                mousePos.y >= closePos_.y - closeH_ / 2 && mousePos.y <= closePos_.y + closeH_ / 2);

        if (InputManager::GetInstance().IsTrgMouseLeft() && isCloseOver) {
            flagImg_ = false;
            // 実績はflag_とは独立しているため、flag_はここでは操作しない
            // 異常が起こっている状態で実績画面を開いても、閉じるときに異常は解除しない
        }

        // 実績ウィンドウが開いている間は他の操作をロックし、進行も止める
        return;
    }

    // --- 2. ミニゲーム開始/実行処理 ---
    if (IsMinigameActive() && pcMinigame_)
    {
        // 2.1. ミニゲームがまだ開始されていない場合
        if (!isGamePlaying_)
        {
            // マウスオーバーしていて、クリックされたらゲーム開始
            if (GetIsMouseOver() && InputManager::GetInstance().IsTrgMouseLeft())
            {
                flag_ = false;                  // PCアクティブフラグをOFF
                SetMinigameActive(false);       // ミニゲーム準備完了フラグをOFF (ネコを解放)
                isGamePlaying_ = false;         // ゲーム中フラグをOFF 
                flagLevel_ = 0;                 // 進行度をリセット
                progressTimer_ = 0;             // 進行タイマーをリセット

                return;
            }
        }
        // 2.2. ミニゲーム実行中の場合
        else // if (isGamePlaying_)
        {
            bool isGameCleared = pcMinigame_->UpdateGame();

            if (isGameCleared)
            {
                // ミニゲームクリア時のリセット処理
                flag_ = false;
                SetMinigameActive(false);
                isGamePlaying_ = false;
                flagLevel_ = 0;
                progressTimer_ = 0;
            }
            // ミニゲーム中も進行処理は続けるため、returnはしない
        }
    }

    // --- 3. PCクリックでウィンドウを開く (ミニゲーム中でない場合) ---
    // ミニゲームがアクティブまたは実行中でない場合に、実績ウィンドウを開けるようにする
    if (!IsMinigameActive() && !isGamePlaying_)
    {
        if (InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_) {
            flagImg_ = true;
            // flag_がfalseの時に実績ウィンドウを開いたら、flag_はそのままfalse
        }
    }


    // --- 4. PCがアクティブならネコが接近 (異常進行) ---
    if (flag_) {

        progressTimer_++;
        if (progressTimer_ > 300) {
            if (flagLevel_ < maxLevel_)
                flagLevel_++;
            progressTimer_ = 0;
        }
    }
    else {
        progressTimer_ = 0;
    }

    // --- ゲームオーバー判定 ---
    isGameOver_ = (flagLevel_ >= maxLevel_);
}

void PC::Draw(void)
{
    // 通常のPCアイコン描画
    DrawRotaGraph(pos_.x, pos_.y, 2.0, 0.0, img_, true);
    DrawRotaGraph(pos_.x - 100, pos_.y + 150, 0.1, 0.0, img2_, true);

    // flagLevelに応じて描画
    int drawImg = 0;
    if (flag_ && flagLevel_ >= 5) {
        drawImg = imgE_;
    }
    else if (flag_ && flagLevel_ == 4) {
        drawImg = imgD_;
    }
    else if (flag_ && flagLevel_ == 3) {
        drawImg = imgC_;
    }
    else if (flag_ && flagLevel_ == 2) {
        drawImg = imgB_;
    }
    else if (flag_ && flagLevel_ == 1) {
        drawImg = imgA_;
    }

    if (drawImg != 0) {
        DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, drawImg, true);
    }

    if (isGamePlaying_ && pcMinigame_) {
        pcMinigame_->Draw();
    }

    if (isMouseOver_) {
        DrawBox(
            pos_.x - PC_WID / 2.0f, pos_.y - PC_HIG / 2.0f,
            pos_.x + PC_WID / 2.0f, pos_.y + PC_HIG / 2.0f,
            GetColor(255, 0, 0), false);
    }
}

// 実績ウィンドウ専用描画
void PC::DrawAchievementWindow(void)
{
    if (!flagImg_) return;

    // 実績ウィンドウ背景
    DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, infoImg_, true);

    // テキスト or 画像切り替え
    if (flag_) {
        DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.5, 0.0, img3_, true);
    }
    else {
        int textW = GetDrawStringWidth(infoText_.c_str(), (int)infoText_.size());
        DrawFormatString((Application::SCREEN_SIZE_X - textW) / 2, Application::SCREEN_SIZE_Y / 2,
            GetColor(255, 255, 255), infoText_.c_str());
    }

    // 閉じるボタン
    Vector2 mousePos = InputManager::GetInstance().GetMousePos();
    bool isOver = (mousePos.x >= closePos_.x - closeW_ / 2 &&
        mousePos.x <= closePos_.x + closeW_ / 2 &&
        mousePos.y >= closePos_.y - closeH_ / 2 &&
        mousePos.y <= closePos_.y + closeH_ / 2);

    DrawBox(closePos_.x - closeW_ / 2, closePos_.y - closeH_ / 2,
        closePos_.x + closeW_ / 2, closePos_.y + closeH_ / 2, GetColor(57, 53, 48), true);

    if (isOver)
        DrawBox(closePos_.x - closeW_ / 2, closePos_.y - closeH_ / 2,
            closePos_.x + closeW_ / 2, closePos_.y + closeH_ / 2, GetColor(255, 0, 0), false);

    DrawFormatString(closePos_.x - 60, closePos_.y - 10, GetColor(255, 255, 255), "閉じる");
}

void PC::Release()
{
    // 追加の画像を解放
    DeleteGraph(img3_);
    DeleteGraph(infoImg_);

    // ItemBaseのReleaseを呼んで img_ と img2_ を解放
    ItemBase::Release();
}

void PC::SetInfoText(const std::string& text)
{
    infoText_ = text;
}

void PC::ChangeImage()
{
    //img_ = LoadGraph((Application::PATH_ITEM + "pc_on.png").c_str());
}

VECTOR PC::GetTargetPos() const
{
    VECTOR target = pos_;
    //target.y += 200; // ネコの立ち位置例
    return target;
}