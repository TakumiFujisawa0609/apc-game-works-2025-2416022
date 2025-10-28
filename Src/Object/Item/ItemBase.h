#pragma once
#include <DxLib.h>

class ItemBase
{
public:

	// 状態
	enum class STATE
	{
		NONE,
		STANDBY,
		ACT1,
		ACT2,
		DEAD,
		END,
	};

	// エネミー種別
	enum class TYPE
	{
		FOOD,
		TOY,
		WALL,
		PC,
	};

	// コンストラクタ
	ItemBase(void);

	// デストラクタ
	virtual ~ItemBase(void);

	// 初期処理
	void Init(TYPE type, int img);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 状態遷移
	//void ChangeState(STATE state);

	bool GetIsMouseOver() const;

protected:

	// 状態
	STATE state_;

	// 種別
	TYPE type_;

	// モデルのハンドルID
	int img_;
	VECTOR pos_;

	int count_;

	// パラメータ設定(純粋仮想関数)
	virtual void SetParam(void) = 0;

	/*// 状態遷移
	virtual void ChangeStandby(void);
	virtual void ChangeAttack(void);
	virtual void ChangeHitReact(void);
	virtual void ChangeDeadReact(void);
	virtual void ChangeEnd(void);

	// 状態別更新
	virtual void UpdateStandby(void);
	virtual void UpdateAttack(void);
	virtual void UpdateHitReact(void);
	virtual void UpdateDeadReact(void);
	virtual void UpdateEnd(void);

	// 状態別描画
	virtual void DrawStandby(void);
	virtual void DrawAttack(void);
	virtual void DrawHitReact(void);
	virtual void DrawDeadReact(void);
	virtual void DrawEnd(void);*/
};

