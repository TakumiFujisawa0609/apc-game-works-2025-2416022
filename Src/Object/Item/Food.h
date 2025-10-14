#pragma once
#include "../../Common/Vector2.h"
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

    VECTOR GetPos(void) const;
    bool GetFlag(void) const;
    void SetFlag(bool flag) { flag_ = flag; }

private:

    int img_;
    int img2_;

    VECTOR pos_;
    bool flag_;

    int count_;

    int spawnTimer_;      // 次の出現までのカウント
    int spawnInterval_;   // 出現間隔の上限時間（乱数用）

    int spawnTimerBase_ = 180;       // img_ 初期再表示時間（フレーム）
    float spawnTimerMultiplier_ = 1.0f; // img_ 再表示時間短縮係数

    int flagSpawn_;

};
