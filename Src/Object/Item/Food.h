#pragma once
#include <DxLib.h>
//---------------------------------------------
// 餌アイテムクラス
//---------------------------------------------
class Food
{
public:

    static constexpr int FOOD_WID = 64;
    static constexpr int FOOD_HIG = 64;

    Food(void);
    ~Food(void);

    void Init(void) ;                   // 初期化（画像ロードなど）
    void Update(void);                 // アニメーション・時間制御
    void Draw(void) ;                   // 描画処理
    void Release(void);

    bool GetFlag(void) const;

private:

    int img_;

    VECTOR pos_;
    bool flag_;

    int count_;

};
