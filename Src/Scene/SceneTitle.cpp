#include"SceneTitle.h"

#include<DxLib.h>

#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"

SceneTitle::SceneTitle(void)
{
	logo_ = -1;
}

void SceneTitle::Init(void)
{
	logo_ = LoadGraph((Application::PATH_IMAGE + "nc362712.png").c_str());
}

void SceneTitle::Update(void)
{

	//シーン遷移(デバッグ)
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{

		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);

		//処理終了
		return;
	}
}

void SceneTitle::Draw(void)
{
#ifdef _DEBUG

	//デバッグ
	DrawDebug();

#endif // SCENE_DEBUG

	DrawFormatString(0, 0, 0xffffff, "タイトル");

	//タイトルロゴ
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5, 0.0, logo_, true);

	int font = CreateFontToHandle(NULL, 10, 10);

	DeleteFontToHandle(font);
}

void SceneTitle::Release(void)
{
}

void SceneTitle::DrawDebug(void)
{
}



