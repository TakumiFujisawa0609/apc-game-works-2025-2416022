#include "ItemBase.h"
#include "../../Manager/Generic/InputManager.h"
#include <cmath>
#include <cstdlib> // rand, srand for C++

ItemBase::ItemBase(int maxLevel) : maxLevel_(maxLevel)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Release(void)
{
    // img_ のみ解放 (派生クラスで追加の画像を解放する必要がある)
    if (img_ != 0) {
        DeleteGraph(img_);
        img_ = 0;
    }
    if (img2_ != 0) {
        DeleteGraph(img2_);
        img2_ = 0;
    }
}

// マウスオーバー判定 (共通処理)
void ItemBase::checkMouseOver(float halfW, float halfH)
{
    Vector2 mousePos = InputManager::GetInstance().GetMousePos();
    isMouseOver_ =
        (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
            mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);
}

// ランダム再出現処理 (TV/PCのアクティブ化)
void ItemBase::handleSpawning(int baseInterval)
{
    if (flag_) return; // 既にアクティブならスキップ

    spawnTimer_++;
    if (spawnTimer_ >= spawnInterval_)
    {
        flag_ = true; // アクティブ化
        spawnTimer_ = 0;
        // 次の間隔をランダム設定
        spawnInterval_ = baseInterval + rand() % baseInterval;
    }
}

// ネコによる異常進行処理
void ItemBase::handleProgress(float detectionDistance, int framesPerLevel)
{
    if (!flag_ || flagLevel_ >= maxLevel_) return;

    // 距離計算
    float dx = nekoPos_.x - pos_.x;
    float dy = nekoPos_.y - pos_.y; // 派生クラスでpos_を調整して呼び出すことも可能
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < detectionDistance)
    {
        progressTimer_++;
        if (progressTimer_ > framesPerLevel) // 約3秒ごとに1段階進行
        {
            flagLevel_++;
            progressTimer_ = 0;
        }
    }
    else
    {
        progressTimer_ = 0;
    }

    // ゲームオーバー判定
    isGameOver_ = (flagLevel_ >= maxLevel_);
}