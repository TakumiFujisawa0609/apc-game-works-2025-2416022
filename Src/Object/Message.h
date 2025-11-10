#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include "../Common/Vector2.h"


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
	bool GetIsShowingImage() const; // ← 画像表示中フラグ

private:

	VECTOR pos_;

	bool isMouseOver_;
	bool flagImg_;

	int img_;

	int img2_;
	int img3_;
	int img4_;
	int img5_;
	int img6_;

	int imgA_;
	int imgB_;


	std::vector<int> images_;   // 複数画像
	int currentImageIndex_;     // 現在の画像インデックス
	bool isShowingImage_;       // 画像表示中フラグ

	// キャンセルボタン領域
	VECTOR cancelPos_;
	float cancelW_;
	float cancelH_;

	// 次へボタン領域
	VECTOR nextPos_;
	float nextW_;
	float nextH_;

	// 戻るボタン領域
	VECTOR backPos_;
	float backW_;
	float backH_;


};

