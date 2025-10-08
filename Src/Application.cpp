#include"Application.h"

#include<DxLib.h>
#include<EffekseerForDXLib.h>

#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Fps/FpsControll.h"


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
	
#ifdef _DEBUG
	//ウィンドウのサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
#else
	//ウィンドウのサイズ
	SetGraphMode(DEFA_SCREEN_SIZE_X, DEFA_SCREEN_SZIE_Y, 32);
#endif // _DEBUG
	
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

	//エフェクシアの初期化
	InitEffekseer();

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

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

			id = MessageBox(NULL, TEXT("ゲームを終了します。よろしいですか？"), TEXT(""), MB_YESNO | MB_ICONQUESTION);

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

	//エフェクシアの終了
	Effkseer_End();

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

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == 1)
	{
		DxLib_End();
	}
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

}

Application::Application(void)
{
	fps_ = new Fps();
	isInitFail_ = false;
	isReleaseFail_ = false;
}