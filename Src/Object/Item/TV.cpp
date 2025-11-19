#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Common/Vector2.h"
#include "TV.h"
#include "../Minigame/TVMinigame.h"
#include <cstdlib>

TV::TV(void)
{
    spawnInterval_ = spawnTimerBase_; // Baseのメンバーを初期化
    tvMinigame_ = new TVMinigame();
}

TV::~TV(void)
{
    if (tvMinigame_) {
        delete tvMinigame_;
        tvMinigame_ = nullptr;
    }
}

void TV::Init(void)
{
    // ItemBaseのメンバーを初期化
    flag_ = false;
    isMouseOver_ = false;
    isGameOver_ = false;
    spawnTimer_ = rand() % spawnTimerBase_; // ランダム初期化
    flagLevel_ = 0;
    progressTimer_ = 0;

    img_ = LoadGraph((Application::PATH_ITEM + "nc304917.png").c_str()); // TV台座
    img2_ = LoadGraph((Application::PATH_ITEM + "RQsnx7A.png").c_str()); // TV画面（表示時）

    imgA_ = LoadGraph((Application::PATH_ITEM + "ノイズ+血.png").c_str());
    imgB_ = LoadGraph((Application::PATH_ITEM + "画面1.png").c_str());
    imgC_ = LoadGraph((Application::PATH_ITEM + "画面2.png").c_str());
    imgD_ = LoadGraph((Application::PATH_ITEM + "画面3.png").c_str());
    imgE_ = LoadGraph((Application::PATH_ITEM + "画面4.png").c_str());

    pos_.x = 700;
    pos_.y = 450;
}

void TV::Update(void)
{
    // --- 毎フレーム、マウスが壁の上にあるか判定する ---
    checkMouseOver(TV_WID / 2.0f, TV_HIG / 2.0f);

    // --- 再出現処理 ---
    if (!flag_)
    {
        // ItemBaseの関数で処理
        handleSpawning(spawnTimerBase_);
        if (!flag_) return; // 表示されていないときはこれ以上進まない
    }

    if (IsMinigameActive() && tvMinigame_) // ネコ到達 = ミニゲーム準備完了
    {
        if (!isGamePlaying_)
        {
            // マウスオーバーしていて、クリックされたらゲーム開始
            if (GetIsMouseOver() && InputManager::GetInstance().IsTrgMouseLeft())
            {
                flag_ = false;                  // TVの表示フラグをOFF
                SetMinigameActive(false);       // ミニゲーム準備完了フラグをOFF (ネコを解放)
                isGamePlaying_ = false;         // ゲーム中フラグをOFF (元々falseですが念のため)
                flagLevel_ = 0;                 // 進行度をリセット
                progressTimer_ = 0;             // 進行タイマーをリセット

                return;
            }
        }
        else // if (isGamePlaying_)
        {
            bool isGameCleared = tvMinigame_->UpdateGame();

            if (isGameCleared)
            {
                flag_ = false;
                SetMinigameActive(false);
                isGamePlaying_ = false;
                flagLevel_ = 0;
                progressTimer_ = 0;
            }
            // ミニゲーム中も進行処理は続けるため、returnはしない
        }
    }

    // ミニゲーム準備/実行中でない場合
    if (!IsMinigameActive() && !isGamePlaying_&&flag_)
    {
        if (InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_)
        {
        }
    }

    if (flag_/* && IsMinigameActive()*/ ) {
        progressTimer_++;
        if (progressTimer_ > 180) { // 3秒(180f)で1段階進行 (PCと同じ)
            if (flagLevel_ < maxLevel_)
                flagLevel_++;
            progressTimer_ = 0;
        }
    }
    else {
        progressTimer_ = 0;
    }

    // ゲームオーバー判定 (通常はUpdateの最後にあるはずです)
    isGameOver_ = (flagLevel_ >= maxLevel_);

}

void TV::Draw(void)
{
    if (flag_)
        DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img2_, true); // 画面

    // flagLevelに応じてノイズ画面を描画
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

    // 本体
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    // マウスオーバー枠
    if (isMouseOver_) {
        DrawBox(
            pos_.x - TV_WID / 2.0f, pos_.y - TV_HIG / 2.0f,
            pos_.x + TV_WID / 2.0f, pos_.y + TV_HIG / 2.0f,
            GetColor(255, 0, 0), false);
    }
}

void TV::Release(void)
{
    // 追加の画像を解放
    DeleteGraph(imgA_);
    DeleteGraph(imgB_);
    DeleteGraph(imgC_);
    DeleteGraph(imgD_);
    DeleteGraph(imgE_);

    // ItemBaseのReleaseを呼んで img_ と img2_ を解放
    ItemBase::Release();
}

VECTOR TV::GetTargetPos() const
{
    VECTOR target = pos_;
    // 例: TVの足元より少し下にネコを配置するようY座標を調整
    //target.y += 200;
    // 必要であれば、TVに対して横にずらしたい場合は target.x も調整してください
    return target;
}

void TV::ChangeImage()
{
    // 新しい画像に切り替え
    //img_ = LoadGraph((Application::PATH_ITEM + "tv_on.png").c_str());
}