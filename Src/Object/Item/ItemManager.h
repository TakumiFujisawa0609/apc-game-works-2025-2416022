#pragma once
class ItemManager
{

public:
	ItemManager(void);

	~ItemManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	bool GetIsMouseOver() const;

private:
};

