#pragma once
#include "../../Common/Vector2.h"
#include <DxLib.h>
#include <string>

// ItemBaseクラス: TVとPCの共通基底クラス
class ItemBase
{
public:
    ItemBase(int maxLevel = 5);
    virtual ~ItemBase();

    // 純粋仮想関数: 継承先で必ず実装する
    virtual void Init(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    virtual void Release(void);

    // 共通のプロパティ
    VECTOR GetPos(void) const { return pos_; }
    bool GetFlag(void) const { return flag_; }
    void SetFlag(bool flag) { flag_ = flag; }
    bool GetIsMouseOver() const { return isMouseOver_; }
    bool IsGameOver() const { return isGameOver_; }

    void SetMinigameActive(bool isActive) { isMinigameActive_ = isActive; }
    bool IsMinigameActive() const { return isMinigameActive_; }

    // ネコの位置設定
    void SetNekoPos(const VECTOR& nekoPos) { nekoPos_ = nekoPos; }

    // レベル取得 (ItemBaseで進捗度を管理する想定)
    void GetFlagLevel(int& level) const { level = flagLevel_; }

protected:
    // 座標
    VECTOR pos_;

    // 画像ハンドル
    int img_;
    int img2_;

    // 状態フラグ
    bool flag_ = false;       // アイテムがアクティブ（問題発生中）か
    bool isMouseOver_ = false; // マウスカーソルが上にあるか
    bool isGameOver_ = false;  // ゲームオーバー状態か
    bool isMinigameActive_ = false;
    bool isGamePlaying_ = false;

    // ネコ関連の進行度
    VECTOR nekoPos_;        // ネコの位置
    int flagLevel_ = 0;     // 異常の段階レベル (0:正常, 1以上:異常進行)
    int progressTimer_ = 0; // 異常進行用のタイマー
    int maxLevel_ = 5; // 最大進行レベル (PCでは5, TVは5として実装)

    // 再出現/アクティブ化タイマー
    int spawnTimer_ = 0;    // 次の出現までのカウント
    int spawnInterval_ = 0; // 出現間隔の基本値

    // --- 共通の処理関数 ---

    // マウスオーバー判定 (継承先でサイズを指定して呼び出す)
    void checkMouseOver(float halfW, float halfH);

    // ランダム再出現処理
    void handleSpawning(int baseInterval);

    // ネコによる異常進行処理
    void handleProgress(float detectionDistance, int framesPerLevel);
};