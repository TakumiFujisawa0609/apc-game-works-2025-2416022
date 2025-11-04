#pragma once
#include <DxLib.h>

//---------------------------------------------
// 餌アイテムクラス
//---------------------------------------------
class Toy
{
public:

    static constexpr int TOY_WID = 64;
    static constexpr int TOY_HIG = 64;

    Toy(void);
    ~Toy(void);

    void Init(void);                   // 初期化（画像ロードなど）
    void Update(void);                 // アニメーション・時間制御
    void Draw(void);                   // 描画処理
    void Release(void);

    VECTOR GetPos(void) const;
    bool GetFlag(void) const;
    void SetFlag(bool flag) { flag_ = flag; }

    bool GetFlagShadow(void) const;
    void SetFlagShadow(bool flagShadow) { flagShadow_ = flagShadow; }

    bool GetIsMouseOver() const;

    bool IsGameOver() const { return isGameOver_; }

private:

    int img_;
    int img2_;

    VECTOR pos_;
    bool flagImg_;

    bool flag_;

    int count_;

    int spawnTimer_;      // 次の出現までのカウント
    int spawnInterval_;   // 出現間隔の上限時間（乱数用）

    int spawnTimerBase_ = 180;       // img_ 初期再表示時間（フレーム）
    float spawnTimerMultiplier_ = 1.0f; // img_ 再表示時間短縮係数

    int activeTimer_;        // 出現中の累積時間（リセットされない）
    const int activeLimit_ = 600; // 合計10秒（60fps換算）
    bool isGameOver_;

    int flagSpawn_;

    bool isMouseOver_;

    // 落下演出用
    float fallY_;        // 現在の描画Y位置
    float fallSpeed_;    // 落下速度
    bool  isFalling_;    // 落下中フラグ

    // --- 影関連 ---
    int shadowImg_;        // 影の画像
    bool flagShadow_;      // 影を表示するかどうか
    int shadowTimer_;      // 落下までの影予告タイマー
	float shadowAlpha_;    // 影の透明度

};
