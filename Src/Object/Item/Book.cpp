#include "Book.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Application.h"
#include <DxLib.h>
#include <cstdlib>

Book::Book() : ItemBase()
{
}

Book::~Book()
{
}

void Book::Init()
{
    // ItemBaseのメンバーを初期化
    flag_ = false;           // ネコの異常アクティブフラグ
    isMouseOver_ = false;
    isGameOver_ = false;
    flagLevel_ = 0;          // 異常の進行度
    progressTimer_ = 0;      // 異常進行用タイマー

    spawnTimer_ = 900 + rand() % 301;

    // 画像ロード
    img_ = LoadGraph((Application::PATH_ITEM + "Book1.png").c_str());
    imgA_ = LoadGraph((Application::PATH_ITEM + "Book.png").c_str());
    imgB1_ = LoadGraph((Application::PATH_ITEM + "Book1.png").c_str());
    imgB2_ = LoadGraph((Application::PATH_ITEM + "Book2.png").c_str());
    imgB3_ = LoadGraph((Application::PATH_ITEM + "Book3.png").c_str());
    imgB4_ = LoadGraph((Application::PATH_ITEM + "Book4.png").c_str());

    // 位置設定
    pos_ = { 300, 460 };

}

void Book::Update()
{
    // --- マウスオーバー判定 ---
    checkMouseOver(BOOK_WID / 2.0f, BOOK_HIG / 2.0f);

    // --- 再出現処理 ---
    if (!flag_)
    {
        spawnTimer_--;
        if (spawnTimer_ <= 0) {
            flag_ = true;
            spawnTimer_ = 900 + rand() % 301;
        }
    }

    // --- 1. ランダムな画像切り替え処理 ---
    if (InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_) 
    { 
        isTargetImgFlag_ = false;

        // nつの画像間でランダムに切り替える
        currentImageId_ = rand() % 8;
        // 特定の画像になったらフラグを立てる
        if (currentImageId_ == targetImageId_) {
            isTargetImgFlag_ = true;
        }
    }


    // ネコがアイテムに到着するまでの間に、特定の画像になっていなかったらゲームオーバー
    if (flag_&&IsMinigameActive()) {
        // ネコが到着した

        if (isTargetImgFlag_) {
           // 画像を変更して、フラグをリセット（異常解除）
           currentImageId_ = 0; // 画像を通常に戻す
           flag_ = false;
           isTargetImgFlag_ = false;
        }
         else {
         isGameOver_ = true; // ゲームオーバーフラグを立てる
         }
         return;
    }
}


void Book::Draw(void)
{

    int drawImg = img_; // デフォルト画像

    if (currentImageId_ == 1) {
        drawImg = imgA_;
    }
    else if (currentImageId_ == 2) {
        drawImg = imgB1_;
    }
    else if (currentImageId_ == 3) {
        drawImg = imgB2_;
    }
    else if (currentImageId_ == 4) {
        drawImg = imgB3_;
    }
    else if (currentImageId_ == 5) {
        drawImg = imgB4_;
    }

    DrawRotaGraph(pos_.x, pos_.y, 1.0, 0.0, drawImg, true);

    if (isMouseOver_) {
        DrawBox(pos_.x - BOOK_WID / 2.0f, pos_.y - BOOK_HIG / 2.0f,
            pos_.x + BOOK_WID / 2.0f, pos_.y + BOOK_HIG / 2.0f,
            GetColor(255, 0, 0), false);
    }

    //DrawFormatString(0, 30, GetColor(255, 255, 255), "Book ID: %d", currentImageId_);

    // isTargetImgFlag_ の状態も確認したい場合は以下を追加
    //DrawFormatString(0, 50, GetColor(255, 255, 0), "Target Flag: %s", isTargetImgFlag_ ? "TRUE" : "FALSE");

    //DrawFormatString(0, 70, GetColor(255, 255, 255), "flag: %s", flag_ ? "TRUE" : "FALSE");
}

void Book::Release()
{
    DeleteGraph(imgA_);
    DeleteGraph(imgB1_);
    DeleteGraph(imgB2_);
    DeleteGraph(imgB3_);
    DeleteGraph(imgB4_);

    ItemBase::Release();
}

VECTOR Book::GetTargetPos() const
{
    VECTOR target = pos_;
    //target.y += 50; // Bookの前にネコが座る位置
    return target;
}