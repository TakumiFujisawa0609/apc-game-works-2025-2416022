#pragma once

#include<memory>



#include "SceneBase.h"
#include "../Application.h"


class Stage;
class Neko;
class Food;
class Wall;
class Toy;

class Message;

class SceneGame : public SceneBase
{
public:

	enum class GameOverSource
	{
		NONE,
		FOOD,
		WALL,
		TOY,
	} gameOverSource_ = GameOverSource::NONE; // ★どこでゲームオーバーになったかを記録

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

	void StartGameOver(GameOverSource source);

	void DrawInfo(void);

private:

	//描画(デバッグ)
	void DrawDebug(void);

	Stage* stage_;

	Neko* neko_;

	Food* food_;

	Wall* wall_;

	Toy* toy_;

	Message* message_;

	int fontHandle;

	int img3_;
	int img4_;
	int img5_;

	int count_;
	bool isEnd_;

	bool isGameOver_ = false;
	int gameOverTimer_ = 0;
	const int GAMEOVER_WAIT_ = 120; // 約2秒待機

	

};

