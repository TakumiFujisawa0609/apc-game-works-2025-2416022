#pragma once

#include "../../Object/Item/Food.h"
#include "../../Object/Item/Wall.h"
#include "../../Object/Item/PC.h"
#include "../../Object/Item/TV.h"
#include "../../Object/Item/Book.h"

#include <DxLib.h>

class Food;
class PC;
class TV;
class Wall;
class Book;

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
		PC,
		TV,
		BOOK,
		ACT,
		GAMEOVER,
		END,
	};

	// 追従ターゲット
	enum class TARGET
	{
		NONE,
		FOOD,
		PC,
		TV,
		BOOK,
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

	void SetFood(Food* food);

	void SetPC(PC* pc);

	void SetTV(TV* tv);

	void SetBook(Book* book);

	void SelectTarget();

	bool GetIsMouseOver() const;

	VECTOR GetPos() const { return pos_; }	

private:

	float hunger_ = 0.0f;

	static constexpr float MAX_HUNGER = 100.0f; // 満腹度最大値
	static constexpr float HUNGER_DECREASE_MOVE = 0.1f; // 移動中の減少量/フレーム
	static constexpr float HUNGER_RECOVER_EAT = 20.0f; // 摂食中の回復量/フレーム
	static constexpr float MOVE_THRESHOLD = 1.0f; // 移動を許可する最低限の空腹度

	// 状態
	STATE state_;
	TARGET targetType_ = TARGET::NONE;

	int img_;

	VECTOR pos_;

	float moveDirX_;
	float moveDirY_;

	//int frameCounter_;    // アニメーション用カウンタ
	bool isMoving_;       // 移動中か停止中か

	bool justChangedState_;

	int moveTimer_;

	int standbyTimer_ = 0;

	int targetTimer_ = 0;

	TARGET lastTargetType_ = TARGET::NONE;
	int targetCoolDown_ = 0;

	bool isMouseOver_;

	bool isVisible_; // 表示フラグ
	bool justHidden_;

	void Move(void);
	void MoveToTarget(VECTOR targetPos, bool targetFlag);

	// 状態遷移
	void ChangeState(STATE state);
	virtual void ChangeStandby(void);
	virtual void ChangeMove(void);
	virtual void ChangeEat(void);
	virtual void ChangePC(void);
	virtual void ChangeTV(void);
	virtual void ChangeBook(void);
	virtual void ChangeAct(void);
	virtual void ChangeGameover(void);
	virtual void ChangeEnd(void);

	// 状態別更新
	virtual void UpdateStandby(void);
	virtual void UpdateMove(void);
	virtual void UpdateEat(void);
	virtual void UpdatePC(void);
	virtual void UpdateTV(void);
	virtual void UpdateBook(void);
	virtual void UpdateAct(void);
	virtual void UpdateGameover(void);
	virtual void UpdateEnd(void);

	// 状態別描画
	virtual void DrawStandby(void);
	virtual void DrawMove(void);
	virtual void DrawEat(void);
	virtual void DrawPC(void);
	virtual void DrawTV(void);
	virtual void DrawBook(void);
	virtual void DrawAct(void);
	virtual void DrawGameover(void);
	virtual void DrawEnd(void);

	bool ShouldSwitchTarget();
	void OnArriveTarget();
	void DrawCommon();

	Food* food_ = nullptr;

	PC* pc_ = nullptr;

	TV* tv_ = nullptr;

	Wall* wall_ = nullptr;

	Book* book_ = nullptr;
};

