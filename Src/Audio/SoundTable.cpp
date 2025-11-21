#include "SoundTable.h"

// サウンドID から サウンドパスを取得

// システム用
namespace SoundTable_System 
{
	static const std::unordered_map<SoundID, std::string> Table = 
	{
		{ SoundID::BGM_TITLE, "Data/Sound/BGM/system.mp3" },
		{ SoundID::SE_CLICK, "Data/Sound/SE/system.mp3" },
	};
}

// タイトル画面用
namespace SoundTable_Title
{
	static const std::unordered_map<SoundID, std::string> Table =
	{
		{ SoundID::BGM_TITLE, "Data/Sound/BGM/nc422681.mp3" },
		{ SoundID::SE_CLICK, "Data/Sound/SE/nc23432.wav" },
		{ SoundID::SE_TITLE, "Data/Sound/SE/system.mp3" },
	};
}

// ゲーム画面用
namespace SoundTable_Game 
{
	static const std::unordered_map<SoundID, std::string> Table =
	{
		{ SoundID::BGM_GAME, "Data/Sound/BGM/nc134328.wav" },
		{ SoundID::SE_CLICK, "Data/Sound/SE/hit.wav" },
		{SoundID::SE_NEKO,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_EAT,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_FOOD_DELETE,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_FOOD_EAT,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_FOOD_SPAWN,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_PC_BOOT,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_PC_CLEAR,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_PC_GAMEOVER,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_PC_VIRUS,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TV_BOOT,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TV_CLEAR,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TV_GAMEOVER,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TV_PLAY,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_WALL_BOOT,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_WALL_CLEAR,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_WALL_EYE,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TOY_BOOT,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TOY_CLEAR,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TOY_FALL,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TOY_GAMEOVER,"Data/Sound/SE/hit.wav"},
		{SoundID::SE_TOY_MOVE,"Data/Sound/SE/hit.wav"},
	};
}
