
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "Neko.h"

Neko::Neko(void)
{
}

Neko::~Neko(void)
{
}

void Neko::Init(void)
{
	img_= LoadGraph((Application::PATH_MODEL + "Nekoproto.jpg").c_str());

	pos_.x = NEKO_WID / 2 + Application::SCREEN_SIZE_X;
	pos_.y = NEKO_HIG / 2 + Application::SCREEN_SIZE_Y;
}

void Neko::Update(void)
{
}

void Neko::Draw(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X/2, Application::SCREEN_SIZE_Y/2, 0.1, 0.0, img_, true);

	DrawBox(pos_.x - NEKO_WID / 2, pos_.y - NEKO_HIG / 2,
		pos_.x + NEKO_WID / 2, pos_.y + NEKO_HIG / 2,
		GetColor(255, 0, 0), false);

}

void Neko::Release(void)
{
	DeleteGraph(img_);
}
