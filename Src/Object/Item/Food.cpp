
#include "Food.h"
#include "../../Application.h"
#include <DxLib.h>
#include <cmath>

Food::Food(void)
{
    
}

Food::~Food(void)
{
}

void Food::Init(void)
{
    // âaâÊëúÇì«Ç›çûÇ›
    img_ = LoadGraph((Application::PATH_ITEM + "5721.png").c_str());
    
    pos_.x = 300;
    pos_.y = 600;

    flag_ = false;
}

void Food::Update(void)
{
    count_++;
      if (rand() % 100 < 20) // 5% ÇÃämó¶Ç≈èoåª
      {
          DrawBox(pos_.x - FOOD_WID / 2, pos_.y - FOOD_HIG / 2,
              pos_.x + FOOD_WID / 2, pos_.y + FOOD_HIG / 2,
              GetColor(255, 0, 0), false);
      }
}

void Food::Draw(void)
{

    DrawRotaGraph(pos_.x, pos_.y, 0.03, 0.0, img_, true);

    /*DrawBox(pos_.x - FOOD_WID / 2, pos_.y - FOOD_HIG / 2,
        pos_.x + FOOD_WID / 2, pos_.y + FOOD_HIG / 2,
        GetColor(0, 255, 0), false);*/

}

void Food::Release(void)
{
    DeleteGraph(img_);
}

bool Food::GetFlag(void) const
{
    return flag_;
}
