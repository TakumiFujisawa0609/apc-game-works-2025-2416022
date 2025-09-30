#pragma once

#include <DxLib.h>

class Stage
{
public:

	//コンストラクタ
	Stage(void);

	//デストラクタ
	~Stage(void);

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

	int pos_;
};

