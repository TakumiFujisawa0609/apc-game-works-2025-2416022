#pragma once
#include "ItemBase.h"

//---------------------------------------------
// 餌アイテムクラス
//---------------------------------------------
class Toy : public ItemBase
{
private:
    int lifeTimer_;   // 存在してからの経過時間
    int maxLifeTime_; // 存在できる最大時間（超えるとゲームオーバー）

public:
    Toy();
    ~Toy() override = default;

    void Init() override;                   // 初期化（画像ロードなど）
    void Update() override;                 // アニメーション・時間制御
    void Draw() override;                   // 描画処理
    void Spawn(const VECTOR& pos) override; // 生成時の設定
    bool IsExpired() const;                 // ゲームオーバー判定
};
