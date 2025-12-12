#pragma once

#include "ItemBase.h"
#include <DxLib.h>
#include <string>

class Book : public ItemBase
{
public:
    // コンストラクタ
    Book(void);
    // デストラクタ
    ~Book(void) override;

    // 継承された基本メソッド
    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;

    bool GetTargetImg()const { return isTargetImgFlag_; }

    VECTOR GetTargetPos() const; // ネコが接近する位置

private:
    // Book特有の定数
    static constexpr float BOOK_WID = 128.0f;
    static constexpr float BOOK_HIG = 128.0f;

    static constexpr int CHANGE_INTERVAL = 30; // 画像をランダムに切り替える間隔 (30F=0.5秒)
    static constexpr int ARRIVAL_TIME = 180;   // ネコが到着するまでの猶予時間 (180F=3秒)

    bool flagImg_;


    int imgA_; // 特定の画像
    int imgB1_;
    int imgB2_;
    int imgB3_;
    int imgB4_;
    int imgB5_;
    int imgB6_;


    int currentImageId_;      // 現在表示中の画像ID (0:通常, 1:特定画像, 2:別画像, ...)
    int targetImageId_ = 1;   // 特定の画像ID (例: 1)

    bool isTargetImgFlag_ = false; // 特定の画像になっていることを教えるフラグ
    int randomChangeTimer_ = 0;      // 画像切り替え用のタイマー
    int nekoArrivalTimer_ = 0;       // ネコ到着までの猶予時間
    bool nekoApproaching_ = false;   // ネコが接近中かどうかを示すフラグ

    
    

};
