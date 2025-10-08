#include "ItemManager.h"
#include "../../Application.h"

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
ItemManager::ItemManager()
{
    requestTimer_ = 0;
    foodRequested_ = false;
    toyRequested_ = false;
    foodRequestTime_ = 0;
    toyRequestTime_ = 0;
    isGameOver_ = false;
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
ItemManager::~ItemManager()
{
    for (auto* item : items_)
    {
        delete item;
    }
    items_.clear();
}

//-----------------------------------------
// 初期化
//-----------------------------------------
void ItemManager::Init()
{
    RemoveAll();

    auto* food = new Food();
    food->Init();
    items_.push_back(food);

    auto* toy = new Toy();
    toy->Init();
    items_.push_back(toy);

    requestTimer_ = 180 + rand() % 300;
    foodRequested_ = toyRequested_ = false;
    foodRequestTime_ = toyRequestTime_ = 0;
    isGameOver_ = false;
}

//-----------------------------------------
// 更新処理
//-----------------------------------------
void ItemManager::Update()
{
    if (isGameOver_) return;

    // --- 要求発生 ---
    if (--requestTimer_ <= 0 && !foodRequested_ && !toyRequested_)
    {
        RequestRandomItem();
        requestTimer_ = 300 + rand() % 600; // 次の要求までの時間
    }

    // --- 経過時間カウント ---
    if (foodRequested_)
    {
        foodRequestTime_++;
        if (foodRequestTime_ > MAX_WAIT_TIME_)
        {
            isGameOver_ = true;
        }
    }
    if (toyRequested_)
    {
        toyRequestTime_++;
        if (toyRequestTime_ > MAX_WAIT_TIME_)
        {
            isGameOver_ = true;
        }
    }

    // --- アイテム更新 ---
    for (auto* item : items_)
    {
        if (item->IsActive()) item->Update();
    }
}

//-----------------------------------------
// 描画処理
//-----------------------------------------
void ItemManager::Draw()
{
    for (auto* item : items_)
    {
        if (item->IsActive()) item->Draw();
    }
}

//-----------------------------------------
// マウスクリックで要求に応答
//-----------------------------------------
void ItemManager::HandleClick(int mx, int my)
{
    if (isGameOver_) return;

    if (foodRequested_)
    {
        VECTOR pos = { (float)mx, (float)my, 0.0f };
        SpawnItem(ItemType::Food, pos);
        foodRequested_ = false;
        foodRequestTime_ = 0;
    }
    else if (toyRequested_)
    {
        VECTOR pos = { (float)mx, (float)my, 0.0f };
        SpawnItem(ItemType::Toy, pos);
        toyRequested_ = false;
        toyRequestTime_ = 0;
    }
}

//-----------------------------------------
// アイテム生成
//-----------------------------------------
void ItemManager::SpawnItem(ItemType type, const VECTOR& pos)
{
    for (auto* item : items_)
    {
        if (item->GetType() == type && !item->IsActive())
        {
            item->Spawn(pos);
            break;
        }
    }
}

//-----------------------------------------
// アイテムリセット（全削除）
//-----------------------------------------
void ItemManager::RemoveAll()
{
    for (auto* item : items_)
    {
        delete item;
    }
    items_.clear();
}

//-----------------------------------------
// ランダムで要求を発生させる
//-----------------------------------------
void ItemManager::RequestRandomItem()
{
    if (rand() % 2 == 0)
    {
        foodRequested_ = true;
        foodRequestTime_ = 0;
    }
    else
    {
        toyRequested_ = true;
        toyRequestTime_ = 0;
    }
}
