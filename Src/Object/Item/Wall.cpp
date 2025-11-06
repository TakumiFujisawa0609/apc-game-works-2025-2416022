#include "Wall.h"
#include "../../Application.h"
#include "../../Scene/SceneTitle.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Common/Vector2.h"

#include <DxLib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>



Wall::Wall(void) 
{
}

Wall::~Wall(void) 
{
}

void Wall::Init(void)
{
    srand((unsigned int)time(nullptr));

    img_ = LoadGraph((Application::PATH_ITEM + "nc73263_穴.png").c_str());
    img2_ = LoadGraph((Application::PATH_ITEM + "nc257030.png").c_str());
    
    img4_ = LoadGraph((Application::PATH_ITEM + "nc407592.png").c_str());

    pos_.x = 600;
    pos_.y = 200;

    flag_ = false;

    isMouseOver_ = false;

    //flagImg_ = true;    // ← img_ を初期表示させる
    flagSpawn_ = false;
    spawnTimer_ = 180 + rand() % 300; // 初回3〜8秒
    count_ = 0;

    spawnInterval_ = 150; // 初期の出現間隔（長め）
    lastSpawnTime_ = 0;

    spawnPositions_.clear();
}

void Wall::Update()
{
    // --- 毎フレーム、マウスが壁の上にあるか判定する ---
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = WALL_WID / 2.0f;
        float halfH = WALL_HIG / 2.0f;

        isMouseOver_ =
            (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
                mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);
    }

    if (flagImg_ && InputManager::GetInstance().IsTrgMouseLeft())
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = WALL_WID / 2.0f;
        float halfH = WALL_HIG / 2.0f;

        if (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
            mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH)
        {
            flagImg_ = false;
            flagSpawn_ = false;
            spawnPositions_.clear();

            // 次の再表示までのランダム時間を短縮
            int randomOffset = rand() % 300;
            spawnTimer_ = (int)((spawnTimerBase_ + randomOffset) / spawnTimerMultiplier_);

            // 再表示ごとに加速
            spawnSpeedMultiplier_ *= 2.0f; // img2_ 出現間隔を短縮
            spawnTimerMultiplier_ *= 2.0f; // img_ 再表示までの時間短縮
            img2StartMultiplier_ *= 2.0f;  // img2_ 出現開始までの時間短縮

            count_ = 0;
            return;
        }
    }



    // --- spawnTimer_ により img_ 再表示 ---
    if (!flagImg_)
    {
        spawnTimer_--;
        if (spawnTimer_ <= 0)
        {
            flagImg_ = true;
            flagSpawn_ = true;
            count_ = 0; // img2_ カウントリセット
        }
    }

    // --- img2_ 出現処理 ---
    if (flagImg_ && flagSpawn_)
    {
        count_++;

        // img2_ 出現開始タイミング
        int startCount = (int)(img2StartCount_ / img2StartMultiplier_);
        if (count_ >= startCount)
        {
            // 出現間隔計算（短縮）
            int baseInterval = (count_ < 1500) ? (int)(spawnIntervalBase_ - 145.0f * (count_ - 600) / (1500 - 600)) : 5;
            spawnInterval_ = (baseInterval / spawnSpeedMultiplier_ > 5) ? (int)(baseInterval / spawnSpeedMultiplier_) : 5;

            // img2_ 出現
            if ((count_ - startCount) % spawnInterval_ == 0)
            {
                VECTOR newPos;
                newPos.x = 600 + (rand() % WALL_WID) - WALL_WID / 2;
                newPos.y = 200 + (rand() % WALL_HIG) - WALL_HIG / 2;
                spawnPositions_.push_back(newPos);
            }
        }

        // ゲームオーバー判定
        if ((int)spawnPositions_.size() >= GAMEOVER_THRESHOLD)
        {
            isGameOver_ = true;
        }
    }

}



void Wall::Draw(void)
{

    float halfW = WALL_WID / 2.0f;
    float halfH = WALL_HIG / 2.0f;

    // --- img_ の描画 ---
    if (flagImg_)
    {
        DrawRotaGraph(pos_.x+10, pos_.y+10, 0.3, 0.0, img_, true);
        
    }

    // --- img2_ の描画 ---
    if (flagSpawn_ && !spawnPositions_.empty())
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
        for (const auto& p : spawnPositions_)
        {
            DrawRotaGraph((int)p.x, (int)p.y, 0.03, 0.0, img2_, true);
        }
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    int color = flagImg_ ? GetColor(255, 0, 0) : GetColor(0, 255, 0);

    if (isMouseOver_) {
        DrawBox(
            pos_.x - halfW, pos_.y - halfH,
            pos_.x + halfW, pos_.y + halfH,
            color, false);

    }
}

void Wall::Release(void)
{
    DeleteGraph(img_);
    DeleteGraph(img2_);
    DeleteGraph(img3_);
    DeleteGraph(img4_);
    spawnPositions_.clear();
}

bool Wall::GetFlagImg(void) const
{
    return flagImg_;
}

bool Wall::GetIsMouseOver() const
{
    return isMouseOver_;
}
