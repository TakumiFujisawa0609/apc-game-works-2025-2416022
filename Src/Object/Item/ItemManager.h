#pragma once
#include <vector>
#include <DxLib.h>
#include "ItemBase.h"
#include "Food.h"
#include "Toy.h"

//-----------------------------------------
// アイテム全体を管理するクラス
//-----------------------------------------
class ItemManager
{
private:
    std::vector<ItemBase*> items_;

    int requestTimer_;       // ランダム要求の発生タイマー
    bool foodRequested_;     // 餌の要求フラグ
    bool toyRequested_;      // おもちゃの要求フラグ
    int foodRequestTime_;    // 餌要求が立ってからの経過時間
    int toyRequestTime_;     // おもちゃ要求が立ってからの経過時間

    const int MAX_WAIT_TIME_ = 600; // 約10秒でゲームオーバー（60fps）

    bool isGameOver_;        // ゲームオーバーフラグ

public:
    ItemManager();
    ~ItemManager();

    void Init();              // 初期化
    void Update();            // 更新処理
    void Draw();              // 描画処理
    void HandleClick(int mx, int my); // マウスクリック対応

    // 状態取得
    bool IsFoodRequested() const { return foodRequested_; }
    bool IsToyRequested() const { return toyRequested_; }
    bool IsGameOver() const { return isGameOver_; }

    // アイテム操作
    void SpawnItem(ItemType type, const VECTOR& pos);
    void RemoveAll();

private:
    void RequestRandomItem(); // ランダム要求処理
};
