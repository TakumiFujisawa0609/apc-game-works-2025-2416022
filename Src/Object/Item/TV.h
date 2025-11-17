#pragma once
#include "ItemBase.h" // 基底クラスをインクルード
#include <DxLib.h>

class TV : public ItemBase // ItemBaseを継承
{
public:

	static constexpr int TV_WID = 132;
	static constexpr int TV_HIG = 132;

	TV(void);
	~TV(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override; // BaseのReleaseをオーバーライドして追加の画像を解放

	// GetPos, GetFlag, SetFlag, GetIsMouseOver, IsGameOver, SetNekoPos は ItemBase に移動

	VECTOR GetTargetPos() const;
	void ChangeImage(); // 画像切替（継承元のimg_を書き換える）

private:
	// 個別の画像ハンドル (img_, img2_ は ItemBase へ移動)
	int imgA_;
	int imgB_;
	int imgC_;
	int imgD_;
	int imgE_;

	// ItemBaseで宣言されたメンバーを流用:
	// VECTOR pos_;
	// bool flag_;
	// bool isMouseOver_;
	// bool isGameOver_;
	// int spawnTimer_;
	// int flagLevel_;
	// VECTOR nekoPos_;

	// TV個別の再出現タイマー定数
	const int spawnTimerBase_ = 180; // ItemBaseのspawnInterval_の初期値に使用
};