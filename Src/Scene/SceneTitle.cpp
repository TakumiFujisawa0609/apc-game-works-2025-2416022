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

	int Cr = GetColor(255, 255, 255);

	int Title = DrawBox(Application::SCREEN_SIZE_X/2, Application::SCREEN_SIZE_Y / 2, 640, 480, Cr, TRUE);    // 四角形を描画

	//タイトルロゴ
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, logo_, true);

	int font = CreateFontToHandle(NULL, 10, 10);

	DeleteFontToHandle(font);
}

void SceneTitle::Release(void)
{
}

void SceneTitle::DrawDebug(void)
{
}



