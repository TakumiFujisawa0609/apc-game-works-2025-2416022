#include"Font.h"


//シングルトンインスタンスの初期化
Font* Font::instance_ = nullptr;

//コンストラクタ
Font::Font() : defaultFont_(""){}

//デストラクタ
Font::~Font(void)
{
	//全てのフォントハンドルを解放
	for (const auto& font : fontHandles_)
	{
		if (font.second != -1 && font.second != DX_DEFAULT_FONT_HANDLE)
		{
			DeleteFontToHandle(font.second);
		}
	}

	//動的フォントハンドルを解放
	for (const auto& dynamicFont : dynamicFontHandles_)
	{
		if (dynamicFont.second != -1 && dynamicFont.second != DX_DEFAULT_FONT_HANDLE)
		{
			DeleteFontToHandle(dynamicFont.second);
		}
	}
}

void Font::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Font();
	}
	instance_->Init();
}

//シングルトンインスタンス取得
Font& Font::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		Font::CreateInstance();
	}
	return *instance_;
}

//フォントの初期化
void Font::Init(void)
{
}

//フォントの追加
bool Font::AddFont(const std::string& fontId, const std::string& fontPath, int fontSize, int fontWeight, int fontType)
{
	int fontFileSize = FileRead_size(fontPath.c_str());
	int fontFileHandle = FileRead_open(fontPath.c_str());

	//フォント読み込み失敗
	if (fontFileSize <= 0 || fontFileHandle == -1)
	{
		OutputDebugString("フォントが見つかりません \n");

		return false;
	}

	//フォントデータをメモリに読み込む
	void* buffer = new char[fontFileSize];
	FileRead_read(buffer, fontFileSize, fontFileHandle);

	DWORD font_num = 0;
	if (AddFontMemResourceEx(buffer, fontFileSize, NULL, &font_num) == 0)
	{
		OutputDebugString("フォントファイルが読み込めませんでした\n");
		delete[] buffer;
		return false;
	}

	FileRead_close(fontFileHandle);		//フォントファイルを度汁
	delete[] buffer;					//メモリ解放

	//フォントハンドルを生成
	int fontHandle = CreateFontToHandle(fontPath.c_str(), fontSize, fontWeight, fontType);
	if (fontHandle == -1)
	{
		return false;
	}

	fontHandles_[fontId] = fontHandle;  //フォントハンドルを登録
	return true;
}

//フォントの削除
void Font::RemoveFont(const std::string& fontId)
{
	auto it = fontHandles_.find(fontId);
	if (it != fontHandles_.end())
	{
		if (it->second != -1 && it->second != DX_DEFAULT_FONT_HANDLE)
		{
			DeleteFontToHandle(it->second);
		}
		fontHandles_.erase(it);
	}
}

//デフォルトフォントの設定
void Font::SetDefaultFont(const std::string& fontId)
{
	if (fontHandles_.find(fontId) != fontHandles_.end())
	{
		defaultFont_ = fontId;
	}
}

//テキスト描画
void Font::DrawText(const std::string& fontId, int x, int y, const char* text, int color, int fontSize, int fontType)
{
	int fontHandle;

	if (fontSize > 0)
	{
		// 動的フォントサイズを使用
		fontHandle = GetDynamicFontHandle(fontSize, 3, fontType);
	}
	else
	{
		// 登録済みフォントのハンドルを使用
		auto it = fontHandles_.find(fontId);
		fontHandle = (it != fontHandles_.end()) ? it->second : DX_DEFAULT_FONT_HANDLE;
	}

	if (fontHandle != -1)
	{
		DrawFormatStringFToHandle(x, y, color, fontHandle, text);
	}
}

// デフォルトフォントで描画
void Font::DrawDefaultText(int x, int y, const char* text, int color, int fontSize, int fontType)
{
	DrawText(defaultFont_, x, y, text, color, fontSize, fontType);
}

//一時的なフォントを取得または生成
int Font::GetDynamicFontHandle(int fontSize, int fontWeight, int fontType)
{
	auto key = std::make_pair(fontSize, fontType);
	auto it = dynamicFontHandles_.find(key);

	if (it != dynamicFontHandles_.end())
	{
		return it->second;
	}

	//新しいフォントを生成
	int fontHandle = CreateFontToHandle(nullptr, fontSize, fontWeight, fontType);
	if (fontHandle != -1)
	{
		dynamicFontHandles_[key] = fontHandle;
	}
	return fontHandle;
}