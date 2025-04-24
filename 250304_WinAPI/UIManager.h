#pragma once
#include "config.h"
#include "Singleton.h"
#include "UI/VisualStyle.h"
#include "UI/Core/UIContainer.h"

class Entity;
class UIStatusToolbar;
class Player;
class TimerManager;
struct ID2D1HwndRenderTarget;
class UIQuickSlotToolbar;
class UIStatusPanel;
class UITextLogPanel;
class UIEffectManager;
class UIResourceSubManager;
class UIInventory;
class UITopRightUI;
class UIGameOver;

class UIManager : public Singleton<UIManager>
{
private:
	TimerManager* tmMgr; // 절대 삭제 금지
	ID2D1HwndRenderTarget* rdt; // 절대 삭제 금지
	MouseManager* mouseManager;  // 절대 삭제 금지
	UIResourceSubManager* uiResourceManager;

	Player* currentPlayer;
	// vector<IPlayerObserver>
	D2D1::ColorF colors[5] = {
		D2D1::ColorF::White,
		D2D1::ColorF::Red,
		D2D1::ColorF::Yellow,
		D2D1::ColorF::Cyan,
		D2D1::ColorF::Lime
	};

	vector<UIContainerBase*> uiContainers;
	UIEffectManager* uiEffectManager;
	UITextLogPanel* uiTextLogPanel;
	UIStatusToolbar* uiStatusToolbar;
	UIQuickSlotToolbar* uiQuickSlotToolbar;
	UITopRightUI* uiTopRightUI;
	UIStatusPanel* uiStatusPanel;
	UIInventory* uiInventoryPanel;
	UIGameOver* uiGameOver;

	TextStyle defaultLogStyle{
		L"pixel",
		16.0f,
		D2D1::ColorF(D2D1::ColorF::White),
		false,
		DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR
	};

	function<void()> restartCallback;

public:
public:
	void RegisterPlayer(Player* player);
	void RegisterEntity(Entity* entity);
	void UnregisterPlayer(Player* player);
	void UnregisterEntity(Entity* entity);

	void SendLog(const wstring& msg, D2D1_COLOR_F color);
	void SendTextEffect(const std::wstring& text, const D2D1_RECT_F& rect, TextStyle* textStyle = nullptr , EffectStyle* effectStyle = nullptr);

	void SetWorldUISCale(float scale);
	void UseUIItem(int idx);
	
	void SetCurrentPlayer(Player* player) { currentPlayer = player; }

	virtual ~UIManager();

	void SetRestartCallback(std::function<void()> cb);

	UIGameOver* GetUiGameOver() { return uiGameOver; }

	void ClearUiContainers();

	void Init();
	void Update();
	void Render();
	void Release();
};

