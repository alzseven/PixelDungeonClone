#pragma once
#include "GameObject.h"
class Level;
class GameScene : public GameObject
{
private:
	Level* levels[7];
	int currLevel;
	const int testLevel = 6;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void Restart();

	GameScene();
	virtual ~GameScene() {};
};

