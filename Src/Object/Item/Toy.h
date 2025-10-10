#pragma once
#include <DxLib.h>

//---------------------------------------------
// 餌アイテムクラス
//---------------------------------------------
class Toy
{
public:
    Toy(void);
    ~Toy(void);

    void Init(void);                   // 初期化（画像ロードなど）
    void Update(void);                 // アニメーション・時間制御
    void Draw(void);                   // 描画処理
    void Release(void);

    bool GetFlag(void) const;

private:

    int img_;

    VECTOR pos_;
    bool flag_;

    int lifeTimer_;   // 存在してからの経過時間
    int maxLifeTime_; // 存在できる最大時間（超えるとゲームオーバー）
};
