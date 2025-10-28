#pragma once

#include<memory>



#include "SceneBase.h"
#include "../Application.h"


class Stage;
class Neko;
class Food;
class Wall;
class Message;

class SceneGame : public SceneBase
{
public:

	//コンストラクタ
	SceneGame(void);

	//デストラクタ
	~SceneGame() override = default;

	//初期化処理
	void Init(void)override;

	//更新処理
	void Update(void)override;

	//描画処理
	void Draw(void)override;

	//解放処理
	void Release(void)override;

private:

	//描画(デバッグ)
	void DrawDebug(void);

	Stage* stage_;

	Neko* neko_;

	Food* food_;

	Wall* wall_;

	Message* message_;

	int count_;
	bool isEnd_;
};

