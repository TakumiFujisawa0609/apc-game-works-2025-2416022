#pragma once

#include<memory>

#include"SceneBase.h"

class SceneTitle : public SceneBase
{
public:

	//コンストラクタ
	SceneTitle(void);

	//デストラクタ
	~SceneTitle(void) = default;

	//初期化
	void Init(void)override;

	//更新処理
	void Update(void)override;

	//描画処理
	void Draw(void)override;

	//解放処理
	void Release(void)override;

private:

	//タイトルロゴ
	int logo_;

	//描画(デバック)
	void DrawDebug(void);
};
