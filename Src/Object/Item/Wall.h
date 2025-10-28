#pragma once
#include <DxLib.h>
#include <vector>

#include "../../Application.h"


class Wall
{
public:

    static constexpr int WALL_WID = 100;
    static constexpr int WALL_HIG = 100;

    static constexpr int GAMEOVER_THRESHOLD = 50;

    Wall(void);
    ~Wall(void);

    void Init(void);                   // 初期化（画像ロードなど）
    void Update(void);                 // アニメーション・時間制御
    void Draw(void);                   // 描画処理
    void Release(void);

    bool GetFlagImg(void) const;
    bool GetIsMouseOver() const;

	bool IsGameOver() const { return isGameOver_; }

private:

    int img_;
    int img2_;
    int img3_;
    int img4_;

    VECTOR pos_;
    VECTOR pos2_;
    VECTOR pos3_;

    bool flag_;

    bool flagImg_;   // img_ 表示フラグ
    bool flagSpawn_; // img2_ 出現処理開始フラグ

    bool finished_;  // クリックで消したら true

    bool isClicked_ = false;

    bool isMouseOver_;

	bool isGameOver_ = false;

    int count_;

    int spawnTimer_;      // 次の出現までのカウント
    int spawnInterval_;   // 出現間隔の上限時間（乱数用）
    int lastSpawnTime_;

    int spawnIntervalBase_ = 150;    // img2_ 初期出現間隔
    float spawnSpeedMultiplier_ = 1.0f; // img2_ の加速係数

    int spawnTimerBase_ = 180;       // img_ 初期再表示時間（フレーム）
    float spawnTimerMultiplier_ = 1.0f; // img_ 再表示時間短縮係数

    int img2StartCount_ = 600;          // img2_ 出現開始カウント（初期）
    float img2StartMultiplier_ = 1.0f;  // 再表示ごとに短くする係数

    std::vector<VECTOR> spawnPositions_;

};

