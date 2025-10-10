
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "ItemBase.h"

ItemBase::ItemBase(void)
{
}

ItemBase::~ItemBase(void)
{
}

void ItemBase::Init(TYPE type,int img)
{
	// エネミー種別
	type_ = type;

	// パラメータ設定
	SetParam();

	// 初期状態
	ChangeState(STATE::STANDBY);
}

void ItemBase::Update(void)
{
}

void ItemBase::Draw(void)
{
}

void ItemBase::Release(void)
{
}
