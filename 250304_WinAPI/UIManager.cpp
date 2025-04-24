#include "UIManager.h"

#include "Player.h"
#include "UI/Panel/UITextLogPanel.h"
#include "UI/Panel/UIStatusPanel.h"
#include "UI/UIEffectManager.h"
#include "UI/Panel/UIInventory.h"
#include "UI/Panel/UIQuickSlotToolbar.h"
#include "UI/Panel/UIStatusTollbar.h"
#include "UI/Panel/UITopRightUI.h"
#include "Inventory.h"
#include "GameOver.h"

void UIManager::RegisterPlayer(Player* player)
{
    if (currentPlayer == player) return;
    
    if (currentPlayer)
    {
        UnregisterPlayer(currentPlayer);
    }

    currentPlayer = player;
    player->GetEntityObserverHub().AddObserver(uiStatusToolbar);
    player->GetEntityObserverHub().AddObserver(uiStatusPanel);
    player->GetEntityObserverHub().AddObserver(uiQuickSlotToolbar);
    player->GetEntityObserverHub().AddObserver(uiInventoryPanel);
    player->GetEntityObserverHub().AddObserver(uiGameOver);
}

void UIManager::RegisterEntity(Entity* entity)
{
    entity->GetEntityObserverHub().AddObserver(uiEffectManager);
}

void UIManager::UnregisterPlayer(Player* player)
{
    player->GetEntityObserverHub().RemoveObserver(uiStatusToolbar);
    player->GetEntityObserverHub().RemoveObserver(uiStatusPanel);
    player->GetEntityObserverHub().RemoveObserver(uiQuickSlotToolbar);
    player->GetEntityObserverHub().RemoveObserver(uiInventoryPanel);
    player->GetEntityObserverHub().RemoveObserver(uiGameOver);
}

void UIManager::UnregisterEntity(Entity* entity)
{
    entity->GetEntityObserverHub().RemoveObserver(uiEffectManager);
}


void UIManager::SendLog(const wstring& msg, D2D1_COLOR_F color)
{
    if (uiTextLogPanel)
    {
        defaultLogStyle.color = color;
        uiTextLogPanel->AddLog(msg, defaultLogStyle);
    }
}

void UIManager::SendTextEffect(const std::wstring& text, const D2D1_RECT_F& rect, TextStyle* textStyle,
                               EffectStyle* effectStyle)
{
    if (uiEffectManager)
    {
        uiEffectManager->AddEffect(text, *textStyle, rect, *effectStyle);
    }
}

void UIManager::SetWorldUISCale(float scale)
{
    
}

void UIManager::UseUIItem(int idx)
{
    auto inven = currentPlayer->GetInven();
    if (inven)
    {
        inven->UseItem(idx);
        currentPlayer->GetEntityObserverHub().NotifyChangePlayerInven(currentPlayer);
    }
}

UIManager::~UIManager()
{
}

void UIManager::SetRestartCallback(std::function<void()> cb)
{
    restartCallback = cb;
}

void UIManager::ClearUiContainers()
{
    uiContainers.clear();
}

void UIManager::Init()
{
    tmMgr = TimerManager::GetInstance();
    rdt = D2DImage::GetRenderTarget();
    mouseManager = MouseManager::GetInstance();

    UIResourceSubManager::Preload_NinePatch();

    ClearUiContainers();

    /* 기본 Load */
    uiStatusToolbar = new UIStatusToolbar();
    uiQuickSlotToolbar = new UIQuickSlotToolbar();
    uiTopRightUI = new UITopRightUI();
    uiStatusPanel = new UIStatusPanel();
    uiInventoryPanel = new UIInventory();
    uiTextLogPanel = new UITextLogPanel();
    uiEffectManager = new UIEffectManager();
    uiGameOver = new UIGameOver(); 
    uiGameOver->Init(); 
    uiGameOver->SetActive(false);
    uiStatusToolbar->Init();
    uiTopRightUI->Init();
    uiStatusPanel->Init();
    uiStatusPanel->SetActive(false);
    uiInventoryPanel->Init();
    uiInventoryPanel->SetActive(false);
    uiQuickSlotToolbar->Init();
    uiTextLogPanel->Init();

    /* UIContainers 버튼 연결 */
    uiQuickSlotToolbar->SetActionOnClick(0, [this](){});
    uiQuickSlotToolbar->SetActionOnClick(1, [this](){});
    uiQuickSlotToolbar->SetActionOnClick(2, [this]()
    {
        uiInventoryPanel->SetActive(true);
    
        float mx = mouseManager->GetMousePos().x;
        float my = mouseManager->GetMousePos().y;
    
        D2D1_RECT_F effectRect = D2D1::RectF(mx - 10, my - 10, mx + 10, my + 10);

        TextStyle style;
        style.fontName = L"pixel";
        style.fontSize = 14.0f;
        style.color = D2D1::ColorF(D2D1::ColorF::White);
        style.bold = true;
        style.horizontalAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
        style.verticalAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
        EffectStyle effectStyle;

        SendTextEffect(L"Click", effectRect,&style, &effectStyle);
        SendLog(L"Inventory Quick Click", D2D1::ColorF(D2D1::ColorF::Yellow));
    });

    /* Add UIContainers */
    uiContainers.push_back(uiStatusToolbar);
    uiContainers.push_back(uiQuickSlotToolbar);
    uiContainers.push_back(uiTopRightUI);
    uiContainers.push_back(uiTextLogPanel);
    uiContainers.push_back(uiStatusPanel);
    uiContainers.push_back(uiInventoryPanel);
    uiContainers.push_back(uiGameOver);
    
}

void UIManager::Update()
{
    /* Mouse Click Check */
    if (MouseManager::GetInstance()->GetClickValueUsed() == false)
    {
        float mx = mouseManager->GetMousePos().x;
        float my = mouseManager->GetMousePos().y;
        
        for (auto uiContainer_it = uiContainers.begin(); uiContainer_it != uiContainers.end(); ++uiContainer_it)
        {
            if ((*uiContainer_it)->HandleClick(mx, my))
            {
                MouseManager::GetInstance()->InitPoints();
                MouseManager::GetInstance()->AlreadyClickUsed();
                break;
            }
        }
    }
    /////////

    /* Update */
    float dt = tmMgr->GetDeltaTime();
    for (auto uiContainer_it = uiContainers.begin(); uiContainer_it != uiContainers.end(); ++uiContainer_it)
    {
        (*uiContainer_it)->Update(dt);
    }
    uiEffectManager->Update(dt);
    ///////


    if (KeyManager::GetInstance()->IsOnceKeyDown('K'))
    {
        currentPlayer->TakeDamage(3);
    }
    if (KeyManager::GetInstance()->IsOnceKeyDown('Z'))
    {
        
    }

    if (KeyManager::GetInstance()->IsOnceKeyDown('J'))
    {
            // 샘플 텍스트 목록
            static const std::wstring messages[] = {
                L"Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world!",
                L"🔥 Critical Hit! 🔥 Critical Hit! 🔥 Critical Hit!\n 🔥 Critical Hit!🔥 Critical Hit!",
                L"You picked up a sword.",
                L"Damage: -25",
                L"Enemy defeated!",
                L"EXP +15",
                L"Level up!"
            };

            // 무작위 색상
            D2D1::ColorF colors[] = {
                D2D1::ColorF::White,
                D2D1::ColorF::Red,
                D2D1::ColorF::Yellow,
                D2D1::ColorF::Cyan,
                D2D1::ColorF::Lime
            };

            int i = rand() % 7;
            int c = rand() % 5;

            TextStyle style;
            style.fontName = L"pixel";
            style.fontSize = 18.0f;
            style.color = colors[c];
            style.horizontalAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
            style.verticalAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

            UIManager::GetInstance()->SendLog(messages[i], colors[c]);
    }
}

void UIManager::Render()
{
    /* Render 순서 주의 */
    for (auto uiContainer_it = uiContainers.begin(); uiContainer_it != uiContainers.end(); ++uiContainer_it)
    {
        (*uiContainer_it)->Render(rdt);
    }
    uiEffectManager->Render(rdt);
    
}

void UIManager::Release()
{
    if (uiStatusPanel) delete uiStatusPanel;
    if (uiQuickSlotToolbar) delete uiQuickSlotToolbar;
    if (uiTextLogPanel) delete uiTextLogPanel;
    if (uiEffectManager) delete uiEffectManager;
    if (uiGameOver) delete uiGameOver;
    
    /* 절대 삭제 금지 */
    tmMgr = nullptr;
    rdt = nullptr;
}

// void UIManager::RegisterPlayer(Player* player) {
//     currentPlayer = player;
//     for (auto* obs : playerObservers)
//         player->AddObserver(obs);
// }
//
// void UIManager::UnregisterPlayer(Player* player) {
//     for (auto* obs : playerObservers)
//         player->RemoveObserver(obs);
//     currentPlayer = nullptr;
// }
//
// void UIManager::AddPlayerObserver(IPlayerObserver* observer) {
//     playerObservers.push_back(observer);
//     if (currentPlayer)
//         currentPlayer->AddObserver(observer);
// }
//
// void UIManager::RemovePlayerObserver(IPlayerObserver* observer) {
//     playerObservers.erase(std::remove(playerObservers.begin(), playerObservers.end(), observer), playerObservers.end());
//     if (currentPlayer)
//         currentPlayer->RemoveObserver(observer);
// }
