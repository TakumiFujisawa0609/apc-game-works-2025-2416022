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

	// 拡大率
	scale_ = 0.5f;

	// 実際に描画されるサイズ
	drawW_ = (int)(logoX_ * scale_);
	drawH_ = (int)(logoY_ * scale_);

	// 縮小枠のマージン
	margin_ = 30; // 小さくしたい幅

	// 中心座標
	cx = (Application::SCREEN_SIZE_X / 2) - drawW_ / 2 + margin_ - 5;
	cy = (Application::SCREEN_SIZE_Y / 2) - drawH_ / 2 + margin_ - 2;
	cx1 = (Application::SCREEN_SIZE_X / 2) + drawW_ / 2 - margin_ + 8;
	cy1 = (Application::SCREEN_SIZE_X / 2) + drawH_ / 2 - margin_ - 125;
}

void SceneTitle::Update(void)
{
	// 入力判定
	Vector2 mousePos = InputManager::GetInstance().GetMousePos();

	bool isHit =
		(mousePos.x >= cx && mousePos.x <= cx1 &&
			mousePos.y >= cy && mousePos.y <= cy1);

	//シーン遷移
	if (isHit && InputManager::GetInstance().IsTrgMouseLeft())
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


	// 枠を描画
	DrawBox(
		cx,  // 左上X
		cy,  // 左上Y
		cx1,  // 右下X
		cy1,  // 右下Y
		GetColor(255, 0, 0),
		FALSE
	);

	int font = CreateFontToHandle(NULL, 10, 10);

	DeleteFontToHandle(font);
}

void SceneTitle::Release(void)
{
	DeleteGraph(logo_);
}

void SceneTitle::DrawDebug(void)
{
}



