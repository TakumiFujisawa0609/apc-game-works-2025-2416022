#pragma once
#include "../../Common/Vector2.h"
#include <DxLib.h>

class TV
{
public:

    static constexpr int TV_WID = 132;
    static constexpr int TV_HIG = 132;

	TV(void);
	~TV(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;
	bool GetFlag(void) const;
	//void SetFlag(bool flag) { flag_ = flag; }

	bool GetIsMouseOver() const;

    bool IsGameOver() const;

    void SetNekoPos(const VECTOR& nekoPos);

	void GetFlagLevel(int& level) const { level = flagLevel_; }

private:

    int img_;
    int img2_;

	int imgA_;
	int imgB_;
	int imgC_;
	int imgD_;
	int imgE_;


    VECTOR pos_;
    bool flagImg_;

    bool flag_;

    int count_;

    // 再出現用タイマー
    int spawnTimer_;      // 次の出現までのカウント
    int spawnInterval_;   // 出現間隔の上限時間（乱数用）
    int spawnTimerBase_ = 180;       // img_ 初期再表示時間（フレーム）

    int activeTimer_;        // 出現中の累積時間（リセットされない）
    const int activeLimit_ = 600; // 合計10秒（60fps換算）
    bool isGameOver_;

    int flagSpawn_;

    bool isMouseOver_;

    int flagLevel_ = 0;
    int progressTimer_ = 0;
    VECTOR nekoPos_;
};

