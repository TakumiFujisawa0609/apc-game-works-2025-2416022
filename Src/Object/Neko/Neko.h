#pragma once
#include <DxLib.h>

class Neko
{
public:

	static constexpr int NEKO_WID = 128;
	static constexpr int NEKO_HIG = 128;

	// 状態
	enum class STATE
	{
		NONE,
		STANDBY,
		MOVE,
		EAT,
		ACT,
		GAMEOVER,
		END,
	};

	//コンストラクタ
	Neko(void);

	//デストラクタ
	~Neko(void);

	//初期化処理
	void Init(void);

	//更新処理
	void Update(void);

	//描画処理
	void Draw(void);

	//解放処理
	void Release(void);

private:

	int img_;

	VECTOR pos_;

};

