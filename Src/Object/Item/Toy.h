#pragma once
#include <DxLib.h>

// 前方宣言
class Food;

//---------------------------------------------
// おもちゃアイテムクラス
//---------------------------------------------
class Toy
{
public:
    static constexpr int TOY_WID = 64;
    static constexpr int TOY_HIG = 64;

    // Toyの状態
    enum class State {
        WAITING,      // 待機中（通常状態）
        MOVING,       // Foodに向かって移動中
        EATING,       // 餌を食べている
        RETURNING,    // 元の位置に戻る
        GAME_OVER     // ゲームオーバー
    };

    Toy(void);
    ~Toy(void);
    void Init(void);
    void Update(Food* food = nullptr);  // Foodへの参照を受け取る
    void Draw(void);
    void Release(void);

    VECTOR GetPos(void) const;
    bool GetFlag(void) const;
    void SetFlag(bool flag) { flag_ = flag; }
    bool GetFlagShadow(void) const;
    void SetFlagShadow(bool flagShadow) { flagShadow_ = flagShadow; }
    bool GetIsMouseOver() const;
    bool IsGameOver() const { return state_ == State::GAME_OVER; }
    bool IsFalling() const { return isFalling_; }
    bool IsLanded() const { return flagImg_ && !isFalling_; }
    bool IsEating() const { return state_ == State::EATING; }
    State GetState() const { return state_; }

private:
    // 基本情報
    int img_;
    VECTOR pos_;
    bool flagImg_;
    bool flag_;
    int count_;

    // 再出現タイマー
    int spawnTimer_;
    int spawnInterval_;
    int spawnTimerBase_;
    float spawnTimerMultiplier_;
    int returnTimer_;

    bool isMouseOver_;

    // 落下演出用
    float fallY_;
    float fallSpeed_;
    bool isFalling_;

    // 影関連
    int shadowImg_;
    bool flagShadow_;
    int shadowTimer_;
    float shadowAlpha_;

    // Food連携用
    State state_;               // 現在の状態
    VECTOR targetPos_;          // 移動先（Foodの位置）
    float moveSpeed_;           // 移動速度
    int eatingTimer_;           // 餌を食べている時間
    const int EATING_LIMIT = 600;  // 食事制限時間（10秒）
    float shakeOffset_;         // 震えのオフセット
    int shakeCount_;            // 震えのカウント

private:
    void DrawShadow();
    void ResetShadow();
    void UpdateMoving();        // 移動処理
    void UpdateEating();        // 食事処理
    void UpdateGameOver();      // ゲームオーバー処理
};