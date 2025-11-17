#include "PC.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Application.h"
#include <DxLib.h>
#include <cstdlib>

PC::PC()
// ItemBaseの初期化は自動で行われる
    : ItemBase(5), flagImg_(false), closeW_(170), closeH_(40),
    img3_(0), infoImg_(0)
{
    // ItemBaseのメンバーをPCに合わせて初期化
    spawnInterval_ = 300;
}

PC::~PC()
{
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
    img_ = LoadGraph((Application::PATH_ITEM + "nc215732.png").c_str()); // PC本体
    img2_ = LoadGraph((Application::PATH_ITEM + "nc316039.png").c_str()); // PC画面
    img3_ = LoadGraph((Application::PATH_ITEM + "NyanCat.png").c_str()); // 実績画像
    infoImg_ = LoadGraph((Application::PATH_STAGE + "黒背景.png").c_str()); // 実績背景
    infoText_ = "実績情報なし";

    pos_ = { 85, 400 };
    closePos_ = { 900, 700 };
    cancelPos_ = { 0, 0 };
    cancelW_ = cancelH_ = 0;

    spawnTimer_ = rand() % spawnInterval_;
}

void PC::Update()
{
    // --- マウスオーバー判定 ---
    checkMouseOver(PC_WID / 2.0f, PC_HIG / 2.0f);

    // --- ランダムでアクティブに ---
    // 実績ウィンドウが開いていない & アクティブでない場合
    if (!flagImg_ && !flag_) {
        // ItemBaseの関数で処理（PCはTVと異なり再出現時にflag_をfalseに戻さないため、UpdateでhandleSpawningは使用しない）
        spawnTimer_--;
        if (spawnTimer_ <= 0) {
            flag_ = true; // PCをアクティブにする
            spawnTimer_ = spawnInterval_ + rand() % spawnInterval_;
        }
    }

    // --- 実績ウィンドウ表示中 ---
    if (flagImg_) {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        bool isCloseOver =
            (mousePos.x >= closePos_.x - closeW_ / 2 && mousePos.x <= closePos_.x + closeW_ / 2 &&
                mousePos.y >= closePos_.y - closeH_ / 2 && mousePos.y <= closePos_.y + closeH_ / 2);

        if (InputManager::GetInstance().IsTrgMouseLeft() && isCloseOver) {
            flagImg_ = false;
            if (flag_)
                flag_ = false; // アクティブ状態を解除
        }
        return;
    }

    // 1. ミニゲームの有効化が確認されたら
    /*if (IsMinigameActive())
    {
        // 1.1. ミニゲームがまだ開始されていない場合
        if (!isGamePlaying_)
        {
            // マウスオーバーしていて、クリックされたらゲーム開始
            if (GetIsMouseOver() && InputManager::GetInstance().IsTrgMouseLeft())
            {
                isGamePlaying_ = true;
                InitMinigame(); 
            }
        }
        // 1.2. ミニゲーム実行中の場合
        else if (isGamePlaying_)
        {
            // ★ ミニゲームの更新ロジックを実行
            bool isGameCleared = UpdateMinigameLogic();

            if (isGameCleared)
            {
                // ミニゲームクリア時のリセット処理
                flag_ = false;
                SetMinigameActive(false);
                isGamePlaying_ = false;
                flagLevel_ = 0;
                progressTimer_ = 0;
            }
        }
    }*/

    // --- PCクリックでウィンドウを開く ---
    if (InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_) {
        flagImg_ = true;
    }

    // --- PCがアクティブならネコが接近 ---
    if (flag_) {
        // PCはターゲット位置に補正があるため、ItemBaseのhandleProgressを直接使用せず、ローカルで処理
        float targetY = pos_.y + 200.0f; // GetTargetPosと同じ補正

        // 距離計算
        float dx = nekoPos_.x - pos_.x;
        float dy = nekoPos_.y - targetY;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < 150.0f) { // 接近判定範囲
            progressTimer_++;
            if (progressTimer_ > 180) { // 約3秒で段階進行
                if (flagLevel_ < maxLevel_)
                    flagLevel_++;
                progressTimer_ = 0;
            }
        }
        else {
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
    DrawRotaGraph(pos_.x, pos_.y, 0.05, 0.0, img_, true);
    DrawRotaGraph(pos_.x - 100, pos_.y + 150, 0.1, 0.0, img2_, true);

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
    img_ = LoadGraph((Application::PATH_ITEM + "pc_on.png").c_str());
}

VECTOR PC::GetTargetPos() const
{
    VECTOR target = pos_;
    target.y += 200; // ネコの立ち位置例
    return target;
}