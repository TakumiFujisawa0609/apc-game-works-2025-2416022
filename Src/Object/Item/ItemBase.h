#pragma once
#include <DxLib.h>

//-----------------------------------------
// アイテムの種類
//-----------------------------------------
enum class ItemType
{
    Food,
    Toy,
    Unknown
};

//-----------------------------------------
// アイテムの基底クラス
//-----------------------------------------
class ItemBase
{
protected:
    ItemType type_;       // アイテムの種類
    int graphHandle_;     // 画像ハンドル
    VECTOR pos_;          // 座標
    bool active_;         // 表示中フラグ
    int animFrame_;       // アニメーションフレームカウンタ

public:
    ItemBase();
    virtual ~ItemBase();

    virtual void Init();                   // 初期化
    virtual void Update();                 // 更新処理
    virtual void Draw();                   // 描画処理
    virtual void Spawn(const VECTOR& pos); // 生成位置を指定して出現
    virtual void Reset();                  // 非アクティブ化

    // Getter
    ItemType GetType() const { return type_; }
    bool IsActive() const { return active_; }
    VECTOR GetPos() const { return pos_; }
};
