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
	
	GetGraphSize(logo_, &logoX_, &logoY_);
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

	DrawFormatString(0, 0, 0xffffff, "仮タイトルです。張り紙をクリックすると次に進みます。");

	//タイトルロゴ
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5, 0.0, logo_, true);

	// 拡大率
	float scale = 0.5f;

	// 中心座標
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// 実際に描画されるサイズ
	int drawW = (int)(logoX_ * scale);
	int drawH = (int)(logoY_ * scale);

	// 縮小枠のマージン
	int margin = 30; // 小さくしたい幅

	// 枠を描画（少し小さめ）
	DrawBox(
		cx - drawW / 2 + margin-5,  // 左上X
		cy - drawH / 2 + margin-2,  // 左上Y
		cx + drawW / 2 - margin+8,  // 右下X
		cy + drawH / 2 - margin+2,  // 右下Y
		GetColor(255, 0, 0),
		FALSE
	);

	int font = CreateFontToHandle(NULL, 10, 10);

	DeleteFontToHandle(font);
}

void SceneTitle::Release(void)
{
}

void SceneTitle::DrawDebug(void)
{
}



