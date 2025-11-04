#include "Message.h"
#include "../Object/Item/Wall.h"
#include "../DrawUI/Font.h"
#include "../Application.h"

Message::Message(void)
{
	wall_ = nullptr;
}

Message::~Message(void)
{
}

void Message::Init(void)
{
	wall_ = new Wall();
	wall_->Init();

	pos_.x = 0;
	pos_.y = Application::SCREEN_SIZE_Y - 20;
}

void Message::Update(void)
{
	
}

void Message::Draw(void)
{
	
}

void Message::Release(void)
{
}
