#pragma once
#include "ItemBase.h" // 基底クラスをインクルード
#include <DxLib.h>
#include <string>

class PCMinigame;

class PC : public ItemBase // ItemBaseを継承
{
public:

    static constexpr int PC_WID = 132; // TVと同じ値
    static constexpr int PC_HIG = 132;

    PC();
    ~PC();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // PC固有の描画メソッド
    void DrawAchievementWindow(void);

    // PC固有のメソッド
    VECTOR GetTargetPos() const;
    void SetInfoText(const std::string& text);
    bool GetFlagImg() const { return flagImg_; }
    void ChangeImage(); // 画像切替（継承元のimg_を書き換える）

private:


    bool isGamePlaying_ = false;
    PCMinigame* pcMinigame_;

    // ItemBaseに img_, img2_ はある
    int img3_; // NyanCat画像

    bool flagImg_; // 実績ウィンドウ表示フラグ
    int infoImg_; // 実績ウィンドウ背景画像

    std::string infoText_; // 実績ウィンドウのテキスト

    // 閉じるボタン関連
    VECTOR closePos_;
    int closeW_;
    int closeH_;
    VECTOR cancelPos_;
    int cancelW_;
    int cancelH_;

    // ItemBaseで宣言されたメンバーを流用:
    // VECTOR pos_;
    // bool flag_;
    // bool isMouseOver_;
    // bool isGameOver_;
    // int spawnTimer_;
    // int flagLevel_;
    // VECTOR nekoPos_;
};