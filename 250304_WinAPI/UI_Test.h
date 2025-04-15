#pragma once
#include "GameObject.h"

class Image;
class UI_TestScene : public GameObject
{
private:
	Image* backGround;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

