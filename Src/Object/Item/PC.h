#pragma once
#include <DxLib.h>

class PC
{
public:

    static constexpr int PC_WID = 64;
    static constexpr int PC_HIG = 64;

    PC(void);
    ~PC(void);

    void Init(void);                   // 初期化（画像ロードなど）
    void Update(void);                 // アニメーション・時間制御
    void Draw(void);                   // 描画処理
    void Release(void);

    VECTOR GetPos(void) const;
    bool GetFlag(void) const;
    void SetFlag(bool flag) { flag_ = flag; }

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
};

