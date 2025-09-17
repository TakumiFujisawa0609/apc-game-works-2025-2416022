#pragma once
#include<string>
#include<windows.h>
class Fps;

class Application
{
public:
#ifdef _DEBUG //デバックスクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 800;
	static constexpr int SCREEN_SIZE_Y = 600;

#else

	//ウインドサイズ
	static constexpr int SCREEN_SIZE_X = 800;
	static constexpr int SCREEN_SIZE_Y = 600;
#endif // DEBUG

	//フルスクリーンサイズ
	static constexpr int DEFA_SCREEN_SIZE_X = 1920;
	static constexpr int DEFA_SCREEN_SZIE_Y = 1080;

	

	//FPS
	static constexpr int DEFAULT_FPS = 60;
	static constexpr int FRAME_RATE = 1000 / 60;

	//データパス関連
	//----------------------------------------
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_ANIM;
	static const std::string PATH_EFFECT;
	static const std::string PATH_TEXT;
	static const std::string PATH_FONT;
	static const std::string PATH_JSON;
	static const std::string PATH_BGM;
	static const std::string PATH_SE;
	//----------------------------------------

	//明治的にインスタンスを生成する
	static void CreateInstance(void);

	//性的インスタンスの取得
	static Application& GetInstance(void);

	//初期化
	void Init(void);

	//ゲームループ開始
	void Run(void);

	//リソースの破棄
	void Destroy(void);

	//初期化成功/失敗の判定
	bool IsInitFail(void) const;

	//解放成功/失敗の判定
	bool IsReleaseFail(void)const;

private:

	//性的インスタンス
	static Application* instance_;

	//初期化失敗
	bool isInitFail_;

	//解放処理
	bool isReleaseFail_;

	//フレームレート制御
	Fps* fps_;

	//エフェクシアの初期化
	void InitEffekseer(void);

	//デフォルトコンストラクタ private にして
	//外部から生成できない様にする
	Application(void);

	//コピーコンストラクタも同様
	Application(const Application&);

	//デストラクタ
	~Application(void) = default;
};
