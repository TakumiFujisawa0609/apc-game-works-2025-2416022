
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Common/Vector2.h"
#include "TV.h"

TV::TV(void)
{
}

TV::~TV(void)
{
}

void TV::Init(void)
{
	img_ = LoadGraph((Application::PATH_ITEM + "nc304917.png").c_str());
	img2_ = LoadGraph((Application::PATH_ITEM + "RQsnx7A.png").c_str());

	imgA_ = LoadGraph((Application::PATH_ITEM + "ノイズ+血.png").c_str());
	imgB_ = LoadGraph((Application::PATH_ITEM + "ノイズ+赤.png").c_str());
	imgC_ = LoadGraph((Application::PATH_ITEM + "ノイズ+青.png").c_str());
	imgD_ = LoadGraph((Application::PATH_ITEM + "ノイズ+緑.png").c_str());
	imgE_ = LoadGraph((Application::PATH_ITEM + "完全ノイズ.png").c_str());

	pos_.x = 700;
	pos_.y = 450;

	flag_ = false;
	isMouseOver_ = false;

    spawnTimer_ = 0;
    flagSpawn_ = false;
    flagLevel_ = 0;
    progressTimer_ = 0;
}

void TV::Update(void)
{
    // --- 毎フレーム、マウスが壁の上にあるか判定する ---
    {
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float halfW = TV_WID / 2.0f;
        float halfH = TV_HIG / 2.0f;

        isMouseOver_ =
            (mousePos.x >= pos_.x - halfW && mousePos.x <= pos_.x + halfW &&
                mousePos.y >= pos_.y - halfH && mousePos.y <= pos_.y + halfH);
    }

    // --- 再出現処理 ---
    if (!flag_)
    {
        spawnTimer_++;
        if (spawnTimer_ >= spawnTimerBase_)
        {
            flag_ = true;
            spawnTimer_ = 0;
        }
        return; // 表示されていないときはこれ以上進まない
    }

    // --- クリックで一時的に消す（対処） ---
    if (flag_ && InputManager::GetInstance().IsTrgMouseLeft() && isMouseOver_)
    {
        flag_ = false;
        spawnTimer_ = 0;
        return;
    }

    // --- ネコが近いときに異常進行 ---
    if (flag_ && flagLevel_ < 5)
    {
        float dx = nekoPos_.x - pos_.x;
        float dy = nekoPos_.y - pos_.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < 150.0f)
        {
            progressTimer_++;
            if (progressTimer_ > 180) // 約3秒ごとに1段階進行
            {
                flagLevel_++;
                progressTimer_ = 0;
            }
        }
        else
        {
            progressTimer_ = 0;
        }
    }
}

void TV::Draw(void)
{
    if(flag_)
	DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img2_, true);

	if (flag_ && flagLevel_ == 1)
	{
		DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, imgA_, true);
	}
	else if (flag_ && flagLevel_ == 2)
	{
		DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, imgB_, true);
	}
    else if (flag_ && flagLevel_ == 3)
    {
		DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, imgC_, true);
    }
	else if (flag_ && flagLevel_ == 4)
	{
		DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, imgD_, true);
	}
	else if (flag_ && flagLevel_ >= 5)
	{
		DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, imgE_, true);
	}
    DrawRotaGraph(pos_.x, pos_.y, 0.1, 0.0, img_, true);

    if (isMouseOver_) {
        DrawBox(
            pos_.x - TV_WID / 2.0f, pos_.y - TV_HIG / 2.0f,
            pos_.x + TV_WID / 2.0f, pos_.y + TV_HIG / 2.0f,
            GetColor(255, 0, 0), false);
    }
}

void TV::Release(void)
{
    DeleteGraph(img_);
}

VECTOR TV::GetPos(void) const
{
	return pos_;
}

bool TV::GetFlag(void) const
{
	return flag_;
}

bool TV::GetIsMouseOver() const
{
	return isMouseOver_;
}

void TV::SetNekoPos(const VECTOR& nekoPos)
{
    nekoPos_ = nekoPos;
}

bool TV::IsGameOver() const
{
    return (flagLevel_ >= 5);
}