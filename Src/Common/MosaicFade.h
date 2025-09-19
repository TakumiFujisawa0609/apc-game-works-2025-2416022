#pragma once
#include <DxLib.h>

class MosaicFade {
public:
    enum STATE { NONE, FADE_IN, FADE_OUT };

    STATE GetState(void) const;

    bool IsEnd(void)const;

    void SetFade(STATE state);

    void SetAlpha(float alpha);

    virtual void Init(void);

    // 今の画面をキャプチャ
    void CaptureScreen();

    // フェード開始
    void StartFadeIn();   // モザイクからクリアへ
    void StartFadeOut();  // クリアからモザイクへ


    virtual void Update(void);
    virtual void Draw(void);


private:
    STATE state_;
    int blockSize_;
    bool isEnd_;
    int capturedImage_;

    ///暗転・明転用透過度
    float alpha_;

    ///フェード速度
    float spped_;
};

