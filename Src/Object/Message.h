#pragma once
#include <DxLib.h>

class Message
{
public:

	static constexpr int MESSE_WID = 64;
	static constexpr int MESSE_HIG = 64;

	static constexpr float halfW = MESSE_WID / 2.0f;
	static constexpr float halfH = MESSE_HIG / 2.0f;

	//コンストラクタ
	Message(void);

	//デストラクタ
	~Message(void);

	//初期化処理
	void Init(void);

	//更新処理
	void Update(void);

	//描画処理
	void Draw(void);

	//解放処理
	void Release(void);

	bool GetIsMouseOver() const;

private:

	VECTOR pos_;

	bool isMouseOver_;
	bool flagImg_;
};

