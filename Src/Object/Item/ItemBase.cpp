#include "ItemBase.h"

ItemBase::ItemBase()
{
    type_ = ItemType::Unknown;
    graphHandle_ = -1;
    pos_ = VGet(0, 0, 0);
    active_ = false;
    animFrame_ = 0;
}

ItemBase::~ItemBase()
{
    if (graphHandle_ != -1)
    {
        DeleteGraph(graphHandle_);
    }
}

void ItemBase::Init()
{
    // 派生クラスでロードする想定
}

void ItemBase::Update()
{
    if (!active_) return;

    animFrame_++;
    // 派生クラスでアニメーション処理を追加
}

void ItemBase::Draw()
{
    if (!active_ || graphHandle_ == -1) return;

    DrawGraph((int)pos_.x, (int)pos_.y, graphHandle_, TRUE);
}

void ItemBase::Spawn(const VECTOR& pos)
{
    pos_ = pos;
    active_ = true;
    animFrame_ = 0;
}

void ItemBase::Reset()
{
    active_ = false;
}
