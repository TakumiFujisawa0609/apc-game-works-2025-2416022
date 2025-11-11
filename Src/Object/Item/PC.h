#pragma once
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include <string>

class PC
{
public:

	static constexpr int PC_WID = 132;
	static constexpr int PC_HIG = 132;

    PC();
    ~PC();

    void Init();
    void Update();
    void Draw();
	void DrawAchievementWindow();
    void Release();

    void SetInfoText(const std::string& text);
    void ChangeImage();

    VECTOR GetPos() const;
    VECTOR GetTargetPos() const;
    bool GetFlag() const;
	bool SetFlag(bool flag){ flag_ = flag; return flag_; }
	bool GetFlagImg() const { return flagImg_; }
	bool IsGameOver() const { return isGameOver_; }
	void SetNekoPos(const VECTOR& nekoPos) { nekoPos_ = nekoPos; }
    bool GetIsMouseOver() const;

private:
    VECTOR pos_;       // PC座標
    VECTOR closePos_;  // 閉じるボタン座標

    int closeW_;
    int closeH_;

    bool flag_;        // ランダムでアクティブになるフラグ
    bool flagImg_;     // 実績ウィンドウ表示中か
    bool isMouseOver_;

    int spawnInterval_;  // 出現間隔
    int spawnTimer_;     // 出現タイマー

    int count_;          // 使用用途に応じてカウント可能
    int activeTimer_;    // 累積時間カウンタ
    bool isGameOver_;    // ゲームオーバーフラグ

    int img_;            // PC画像
	int img2_;           
	int img3_;           // 異常用画像
    int infoImg_;        // 実績ウィンドウ背景
    std::string infoText_;

    // --- マウス用 ---
    VECTOR cancelPos_;
    int cancelW_;
    int cancelH_;

    VECTOR nekoPos_;        // ネコの現在位置
    int progressTimer_;     // ネコ滞在タイマー
    int flagLevel_;         // ネコ滞在段階（0～5）
    const int maxLevel_ = 5;

    bool flagWindow_ = false; // クリックで開いたPCウィンドウ表示中
    VECTOR windowPos_;        // ウィンドウの表示位置
    int windowW_ = 400;       // ウィンドウ幅
    int windowH_ = 300;       // ウィンドウ高さ
    VECTOR closePosWindow_;   // 閉じるボタン位置
    int closeWWindow_ = 100;
    int closeHWindow_ = 40;

};
