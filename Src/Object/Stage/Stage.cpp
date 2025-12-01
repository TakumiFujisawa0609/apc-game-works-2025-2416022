#include "../../Application.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	img_= LoadGraph((Application::PATH_STAGE + "room.png").c_str());
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	//ステージ
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, img_, true);
}

void Stage::Release(void)
{
	DeleteGraph(img_);
}
