#include "../Application.h"
#include "MosaicFade.h"

MosaicFade::MosaicFade() {
    Init();
}

MosaicFade::~MosaicFade() {
    if (capturedImage_ != -1) {
        DeleteGraph(capturedImage_);
    }
}

void MosaicFade::Init() {
    state_ = NONE;
    blockSize_ = 1;
    isEnd_ = true;
    capturedImage_ = -1;
}

void MosaicFade::CaptureScreen() {
    if (capturedImage_ != -1) {
        DeleteGraph(capturedImage_);
    }
    capturedImage_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, TRUE);
    GetDrawScreenGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, capturedImage_);
}

void MosaicFade::StartFadeIn() {
    state_ = FADE_IN;
    blockSize_ = 64;
    isEnd_ = false;
}

void MosaicFade::StartFadeOut() {
    state_ = FADE_OUT;
    blockSize_ = 1;
    isEnd_ = false;
}

void MosaicFade::Update() {
    if (isEnd_) return;

    switch (state_) {
    case FADE_IN:
        blockSize_ -= 2;
        if (blockSize_ <= 1) {
            blockSize_ = 1;
            isEnd_ = true;
            state_ = NONE;
        }
        break;

    case FADE_OUT:
        blockSize_ += 2;
        if (blockSize_ >= 64) {
            blockSize_ = 64;
            isEnd_ = true;
            state_ = NONE;
        }
        break;
    }
}

void MosaicFade::Draw(int screen) {
    if (isEnd_) return;

    int src = (state_ == FADE_OUT) ? capturedImage_ : screen;
    if (src == -1) return;

    int w = Application::SCREEN_SIZE_X / blockSize_;
    int h = Application::SCREEN_SIZE_Y / blockSize_;

    int tmp = MakeScreen(w, h, TRUE);

    // 縮小
    SetDrawScreen(tmp);
    DrawExtendGraph(0, 0, w, h, src, FALSE);

    // 拡大（モザイク化）
    SetDrawScreen(DX_SCREEN_BACK);
    DrawExtendGraph(
        0, 0,
        Application::SCREEN_SIZE_X,
        Application::SCREEN_SIZE_Y,
        tmp, FALSE
    );

    DeleteGraph(tmp);
}

bool MosaicFade::IsEnd() const {
    return isEnd_;
}
