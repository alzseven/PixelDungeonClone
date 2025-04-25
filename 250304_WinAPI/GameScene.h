#pragma once
#include "GameObject.h"
#include "D2DImage.h"

class Level;
class Player;
class UIManager;
class GameScene : public GameObject
{
private:
	Player* player;
	UIManager* uiManager;
	Level* levels[7];
	int currLevel;
	const int testLevel = 6;
	D2D1_COLOR_F logColor;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void Restart();

	void Ascend();
	void Descend();

	GameScene();
	virtual ~GameScene() {};
};

