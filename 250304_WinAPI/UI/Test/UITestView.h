#pragma once
#include "../Panel/UIStatusTollbar.h"
#include "../Panel/UIQuickSlotToolbar.h"
#include "../Panel/UITopRightUI.h"
#include "UITestHeader.h"
#include "../../config.h"
#include "../Util/UIHelper.h"
#include "../Panel/UIStatusPanel.h"

class UITestView 
{
public:
	UIStatusToolbar statusToolBar;
	UIQuickSlotToolbar quickSlotToolBar;
	UITopRightUI topRightToolBar;
	UITestHeader uiTester;
	UITestInventoryView uiInventoryView;
	UIAutoContainer uiAutoTestMenu;
	UIStatusPanel uiStatPanel;
	UIAutoContainer uiContainer;
	
	POINT mousePoint;
	float mOffset = 10.0f;

	std::vector<IUIInteractable*> invenInteractables;
	std::vector<UIContainerBase*> rootComponents;

public:
	void Init() {
		UIResourceSubManager::Preload_NinePatch();
		NinePatchStyle defaultNinePatchStyle = UIHelper::CreateNinePatchFromSheet("chrome", "chrome", {6.0f, 6.0f});
		uiContainer.Init({0,0,WINSIZE_X,WINSIZE_Y});
		uiContainer.SetLayout(new UIVerticalLayout());

		// 나인패치 이미지 (가변 크기 이미지)
		// for (int i = 0; i < 9; ++i)
		// {
		// 	float rx = rand() % 100;
		// 	float ry = rand() % 100;
		//
		// 	auto u9Img = UIHelper::ApplyNinePathStyle(&uiContainer, {0, 0, rx, ry}, defaultNinePatchStyle);
		// }
		// uiContainer.UpdateLayout();

		
		// 기본 초기화
		statusToolBar.Init();
		quickSlotToolBar.Init();
		topRightToolBar.Init();
		uiTester.InitUI();  // logPanel, floatingText 등 포함
		uiInventoryView.Init();
		uiInventoryView.inventoryPanel->SetActive(false);
		uiAutoTestMenu.Init({100,400,200,0});
		uiStatPanel.Init();

		// 클릭 대상 탐색을 위해 벡터저장
		rootComponents.push_back(&statusToolBar);
		rootComponents.push_back(&quickSlotToolBar);
		rootComponents.push_back(&topRightToolBar);
		rootComponents.push_back(&uiAutoTestMenu);
		rootComponents.push_back(uiInventoryView.inventoryPanel);


		// 테스트 버튼
		UIButtonStyle buttonStyle;
		buttonStyle.background = {D2DImageManager::GetInstance()->FindImage("inventory_slot")};
		buttonStyle.textStyle = { L"pixel", 14.0f, D2D1::ColorF(D2D1::ColorF::White) };
		auto onCLick = [this]()
		{
			statusToolBar.SetActive(!statusToolBar.IsActive());
		};
		auto* button = UIHelper::ApplyButtonStyle(&uiAutoTestMenu,{0,0,200,50},
			buttonStyle, onCLick);
		UIHelper::SetButtonText(*button, L"스탯용", 0);

		// 클릭 태그 가져오기 테스트
		invenInteractables = uiInventoryView.inventoryPanel->GetInteractables();

		// 인벤 띄우기 활성 클릭 등록
		quickSlotToolBar.SetActionOnClick(2, [this]()
			{
				uiInventoryView.inventoryPanel->SetActive(true);
				wstring debugString = (L"클릭 [인벤] 클릭\n");
				UITestEffectManager::GetInstance()->AddEffect(debugString, quickSlotToolBar.GetWorldRect());
			});
	}

	void Release() {
		// 메모리 정리가 필요한 경우 구현
		// 현재는 대부분 new/delete 처리됨 → 생략 가능
	}

	void Update(float dt) {
		// 기본 업뎃
		statusToolBar.Update(dt);
		quickSlotToolBar.Update(dt);
		topRightToolBar.Update(dt);
		uiInventoryView.Update(dt);
		//uiAutoTestMenu.Update(dt);
		uiTester.UpdateUI(dt);  // 로그/이펙트


		// 입력 인식 안되면 A 글자키 입력 후, 마우스 클릭
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
		{
			GetCursorPos(&mousePoint);
			GetCursorPos(&mousePoint); // 화면 기준 마우스 좌표 가져오기
			ScreenToClient(g_hWnd, &mousePoint); // hWnd는 대상 창 핸들

			// // IUIInteractable 인터페이스(태그) 상속하는 애들
			// for (auto interactable : invenInteractables)
			// {
			// 	interactable->HandleClick(mousePoint.x, mousePoint.y);
			// }

			// 클릭 돌리기, 넣은 순서기준 (자식 역순)
			for (auto& rootComponent : rootComponents)
			{
				if (rootComponent->HandleClick(mousePoint.x, mousePoint.y))
				{
					break;
				}
			}
		}
		else if (KeyManager::GetInstance()->IsOnceKeyDown('C'))
		{
			statusToolBar.SetActive(!statusToolBar.IsActive());
		}
	}

	void Render(ID2D1HwndRenderTarget* rt) {

		statusToolBar.Render(rt);
		quickSlotToolBar.Render(rt);
		topRightToolBar.Render(rt);
		uiInventoryView.Render(rt);
		//uiStatPanel.Render(rt);

		//uiAutoTestMenu.Render(rt);
		uiTester.RenderUI(rt);  // 로그/이펙트

		uiContainer.Render(rt);

		// TODO : 지우기
		D2D1_RECT_F rect = { mousePoint.x - mOffset, mousePoint.y - mOffset, mousePoint.x + mOffset, mousePoint.y + mOffset };

		// 🔸 출력 영역 확인용 사각형 (얇은 외곽선)
		ID2D1SolidColorBrush* debugBrush = nullptr;
		rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &debugBrush);
		rt->DrawRectangle(rect, debugBrush, 1.0f);  // 1.0f: 선 두께
		if (debugBrush)
			debugBrush->Release();
	}

	void Reset() {
		Release();
		Init();
	}

	// UI에 대한 접근이 필요한 경우 getter 추가
	UIStatusToolbar* GetStatusToolbar() { return &statusToolBar; }
	UIQuickSlotToolbar* GetQuickSlotToolbar() { return &quickSlotToolBar; }
	UITopRightUI* GetTopRightUI() { return &topRightToolBar; }
};
