#pragma once
#include <DxLib.h>

class Wall;

class Message
{
public:

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

private:

	VECTOR pos_;

	Wall* wall_;
};

