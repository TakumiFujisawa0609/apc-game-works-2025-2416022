
#include "Toy.h"
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include <DxLib.h>
#include <cmath>

Toy::Toy(void)
{
}

Toy::~Toy(void)
{
}

void Toy::Init(void)
{
    // 画像読み込み
    img_ = LoadGraph((Application::PATH_ITEM + "nc279907.png").c_str());

    pos_.x = 900;
    pos_.y = 650;

    flagImg_ = false;
    isMouseOver_ = false;
	flag_ = false;

    spawnTimerBase_ = 300;        // 基本再表示時間
    spawnTimerMultiplier_ = 1.0f; // 初期倍率
    spawnInterval_ = 300;
    spawnTimer_ = 180 + rand() % 300;
    count_ = 0;

    // 累積時間カウンタ
    activeTimer_ = 0;
    isGameOver_ = false;

    fallY_ = pos_.y;
    fallSpeed_ = 0.0f;
    isFalling_ = false;

    shadowImg_ = LoadGraph((Application::PATH_ITEM + "nc305932.png").c_str());
    flagShadow_ = false;
    shadowTimer_ = 0;
	shadowAlpha_ = 0.0f;


}

void Toy::Update(void)
{
    // --- 毎フレーム、マウスが上にあるか判定する ---
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = TOY_WID / 2.0f;
        float halfH = TOY_HIG / 2.0f;

        isMouseOver_ =
            (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
                mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);
    }

    // --- img_ クリック処理 ---
    if (flagImg_ && InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_)
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = TOY_WID / 2.0f;
        float halfH = TOY_HIG / 2.0f;

        if (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
            mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH)
        {
            // img_ を非表示にして再表示タイマーを設定
            flagImg_ = false;
			flagShadow_ = false;
            activeTimer_ = 0;

            // 完全ランダムな再出現タイマー設定（例：3〜8秒）
            spawnTimer_ = 180 + rand() % 300; // 180〜480フレーム（約3〜8秒）
            // 再表示間隔
            spawnTimerMultiplier_ = 1.0f;

            count_ = 0;
            return;
        }
    }

    // --- 再出現タイマー処理 ---
    if (!flagImg_)
    {
        spawnTimer_--;

        flagShadow_ = true;
        shadowAlpha_ +=1.0f;

        // 出現タイマー終了 → Toy出現＋落下開始
        if (spawnTimer_ <= 0)
        {
            flagImg_ = true;
            isFalling_ = true;
            fallY_ = -100;
            fallSpeed_ = 0.0f;
        }
    }

    // --- 落下演出 ---
    if (isFalling_)
    {
        fallSpeed_ += 1.5f; // 重力加速度
        fallY_ += fallSpeed_;

        // 落下中は影を徐々に濃くする
        if (flagShadow_)
        {
            shadowAlpha_ += 2.0f;  // ゆっくり濃く
            if (shadowAlpha_ > 255.0f)
                shadowAlpha_ = 255.0f;
        }

        if (fallY_ >= pos_.y)
        {
            fallY_ = pos_.y;
            isFalling_ = false;

            // 着地したら影を完全に濃くする
            shadowAlpha_ = 255.0f;
        }
    }

}

void Toy::Draw(void)
{

    // --- 影を描画 ---
    if (flagShadow_)
    {
        int alpha = (int)shadowAlpha_;
        if (alpha > 255) alpha = 255;
        if (alpha < 0) alpha = 0;

        // 影の濃さをAlphaブレンドで描画
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawRotaGraph(pos_.x, pos_.y+10 + 10, 0.1, 0.0, shadowImg_, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }



    float halfW = TOY_WID / 2.0f;
    float halfH = TOY_HIG / 2.0f;

    // --- img_ の描画 ---
    if (flagImg_)
    {
        DrawRotaGraph(pos_.x, fallY_, 1.0, 0.0, img_, true);
        float halfW = TOY_WID / 2.0f;
        float halfH = TOY_HIG / 2.0f;

        DrawBox(
            pos_.x - halfW, fallY_ - halfH,
            pos_.x + halfW, fallY_ + halfH,
            GetColor(0, 0, 255), false);
    }

    int color = isMouseOver_ ? GetColor(255, 0, 0) : GetColor(0, 255, 0);

    if (isMouseOver_) {
        DrawBox(
            pos_.x - halfW, pos_.y - halfH,
            pos_.x + halfW, pos_.y + halfH,
            color, false);
    }
}

void Toy::Release(void)
{

    DeleteGraph(img_);
	DeleteGraph(shadowImg_);
}

VECTOR Toy::GetPos(void) const
{
    return pos_;
}

bool Toy::GetFlag(void) const
{
    return flagImg_;
}

bool Toy::GetFlagShadow(void) const
{
    return flagShadow_;
}

bool Toy::GetIsMouseOver() const
{
    return isMouseOver_;
}
