#pragma once

#include <DxLib.h>

class MinigameBase
{
public:
    MinigameBase(int width, int height);
    virtual ~MinigameBase() = default;

    // 純粋仮想関数: 派生クラスで必ず実装するミニゲームの中身
    virtual void Init() = 0;       // ゲーム内初期化
    virtual bool UpdateGame() = 0; // ゲームロジックの更新 (boolはクリア判定)
    virtual void DrawGame() = 0;   // ゲーム中身の描画

    // 共通の外部インターフェース
    void Start(); // 開始時の共通処理 (画面中央に移動など)
    void End();   // 終了時の共通処理 (リセットなど)

    // 全体的な描画 (共通の背景、枠、タイトルなど)
    void Draw();

protected:
    // 画面サイズ・位置に関する共通プロパティ
    int gameWidth_;
    int gameHeight_;
    VECTOR drawPos_; // 画面上の描画開始座標 (中央配置などに使用)

    bool isCleared_ = false;
    // ... その他、共通のタイマーなど
};
