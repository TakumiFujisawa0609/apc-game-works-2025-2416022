#include"Application.h"

#include<DxLib.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Fps/FpsControll.h"
#include "Audio/AudioManager.h"


Application* Application::instance_ = nullptr;

//ファイル指定パス
const std::string Application::PATH_IMAGE = "Data/Title/";
const std::string Application::PATH_MODEL = "Data/Neko/";
const std::string Application::PATH_STAGE = "Data/Stage/";
const std::string Application::PATH_ITEM = "Data/Item/";
const std::string Application::PATH_TEXT = "Data/Text/";
const std::string Application::PATH_FONT = "Data/Font/";
const std::string Application::PATH_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_SE = "Data/Sound/SE/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
		instance_->Init();
	}
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	//アプリケーションの初期設定
	SetWindowText("Neko_to_Shikou_seyo");

	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);

	//ウィンドウのサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	
	ChangeWindowMode(true);

	//非アクティブ状態でも動作する
	SetAlwaysRunFlag(TRUE);

	//DXLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		//エラー処理
		isInitFail_ = true;
		return;
	}

	HWND hwnd = GetMainWindowHandle();  // DxLibのウィンドウハンドルを取得
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~WS_SYSMENU;               // システムメニュー（×ボタン含む）を無効化
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// サウンド
	AudioManager::CreateInstance();
	AudioManager::GetInstance()->Init();

	// シーン管理初期化
	SceneManager::CreateInstance();

	//FPS制御初期化
	fps_->FpsControll_Initialize();
}

void Application::Run(void)
{
	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	MSG msg;

	//ゲームループ
	while (ProcessMessage() == 0)
	{
		//メッセージループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		//システムに処理を返す
		Sleep(1);

		//FPS制御更新処理
		fps_->FpsControll_Update();

		//ESCAPEキーが押されたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			int id;

			id = MessageBox(NULL, TEXT("ゲームを終了します。よろしいですか？"), TEXT("ポーズ中…"), MB_YESNO | MB_ICONQUESTION);

			if (id == IDYES)
			{
				return;
			}

		}
		//更新処理
		inputManager.Update();
		sceneManager.Update();
		
		//描画処理
		sceneManager.Draw();

		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_TAB) == 1)
		{
			fps_->FpsControll_Draw();
		}
		//フロントバッファに書き出し
		ScreenFlip();


		//FPS制御
		fps_->FpsControll_Wait();
		
	}
}

void Application::Destroy(void)
{
	InputManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();

	// サウンド削除
	AudioManager::GetInstance()->DeleteAll();
	AudioManager::DeleteInstance();


	//DXLibの終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}
	
	delete fps_;
	delete instance_;
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}


Application::Application(void)
{
	fps_ = new Fps();
	isInitFail_ = false;
	isReleaseFail_ = false;
}