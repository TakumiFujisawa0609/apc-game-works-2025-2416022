#pragma once
#include <string>
#include <unordered_map>

// サウンドID
enum class SoundID
{
	BGM_TITLE,
	BGM_GAME,
	BGM_GAMEOVER,
	BGM_GAMECLEAR,

	SE_TITLE,
	SE_CLICK,
	SE_EAT,

	SE_NEKO,

	SE_FOOD_SPAWN,
	SE_FOOD_EAT,
	SE_FOOD_DELETE,

	SE_PC_BOOT,
	SE_PC_VIRUS,
	SE_PC_GAMEOVER,
	SE_PC_CLEAR,

	SE_TV_BOOT,
	SE_TV_PLAY,
	SE_TV_GAMEOVER,
	SE_TV_CLEAR,

	SE_WALL_BOOT,
	SE_WALL_EYE,
	SE_WALL_GAMEOVER,
	SE_WALL_CLEAR,

	SE_TOY_FALL,
	SE_TOY_BOOT,
	SE_TOY_MOVE,
	SE_TOY_GAMEOVER,
	SE_TOY_CLEAR,

};

// 読み込むシーン
enum class LoadScene
{
	SYSTEM,		// システム
	TITLE,		// タイトル
	GAME,		// ゲーム
};

// サウンド属性
struct SoundData
{
	std::string path;	// サウンドのパス
};

// サウンドテーブル（名称被りや拡張性の為にクラス化)
class SoundTable
{
public:
	// サウンドテーブルのマップ
	// サウンドID から サウンドパスを取得
	static const std::unordered_map<SoundID, SoundData> Table;
};
