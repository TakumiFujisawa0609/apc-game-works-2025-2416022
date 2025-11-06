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

	// タイトルロゴのサイズ取得
	int logoX_;
	int logoY_;

	float scale_;
	int drawW_;
	int drawH_;
	int margin_;
	int cx;
	int cy;
	int cx1;
	int cy1;

	// 押下中フラグ（静的変数にして一時保持）
	static bool pressedOnButton;

	//描画(デバック)
	void DrawDebug(void);

	int waitTimer_;      // 一定時間経過チェック用
	int movieHandle_;    // ムービーハンドル
	bool isPlayingMovie_; // ムービー再生中かどうか

	enum class STATE {
		TITLE,   // 通常タイトル表示
		MOVIE,   // ムービー再生中
	};
	STATE state_;
};
