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
#include "UI/UIMopHPManager.h"
#include "UI/Bar/UIMopBar.h"

void UIManager::RegisterPlayer(Player* player)
{
    if (!player) return;
    
    if (currentPlayer == player) return;

    SetCurrentPlayer(player);
    
    player->GetEntityObserverHub().AddObserver(uiStatusToolbar);
    player->GetEntityObserverHub().AddObserver(uiStatusPanel);
    player->GetEntityObserverHub().AddObserver(uiQuickSlotToolbar);
    player->GetEntityObserverHub().AddObserver(uiInventoryPanel);
    player->GetEntityObserverHub().AddObserver(uiGameOver);
}

void UIManager::RegisterEntity(Entity* entity)
{
    if (!entity) return;

    if (uiMopHPManager)
    {
        auto mopHPBar = uiMopHPManager->CreateMopHPBar(entity, camera);
        entity->GetEntityObserverHub().AddObserver(mopHPBar);
    }
    entity->GetEntityObserverHub().AddObserver(uiEffectManager);

    if (entity->GetType() != EntityType::PLAYER)
    {
        observerOwnerEntities.insert(entity);
    }
}

void UIManager::UnregisterPlayer(Player* player)
{
    if (!player) return;
    
    player->GetEntityObserverHub().RemoveObserver(uiStatusToolbar);
    player->GetEntityObserverHub().RemoveObserver(uiStatusPanel);
    player->GetEntityObserverHub().RemoveObserver(uiQuickSlotToolbar);
    player->GetEntityObserverHub().RemoveObserver(uiInventoryPanel);
    player->GetEntityObserverHub().RemoveObserver(uiGameOver);

}

void UIManager::UnregisterEntity(Entity* entity)
{
    if (!entity) return;
    
    if (uiMopHPManager)
    {
        uiMopHPManager->DetachMopBar(entity);
    }
    entity->GetEntityObserverHub().RemoveObserver(uiEffectManager);

    observerOwnerEntities.erase(entity);

}

void UIManager::RegisterCamera(Camera* cam)
{
    if (!cam) return;

    this->camera = cam;

    if (uiEffectManager)
    {
        uiEffectManager->SetCamera(camera);
    }
}


void UIManager::SendLog(const wstring& msg, D2D1_COLOR_F color)
{
    if (uiTextLogPanel)
    {
        defaultLogStyle.color = color;
        uiTextLogPanel->AddLog(msg, defaultLogStyle);
    }
}

void UIManager::SendTextEffect(const std::wstring& text, const D2D1_RECT_F& rect,
    TextStyle* textStyle, EffectStyle* effectStyle)
{
    if (uiEffectManager)
    {
        uiEffectManager->AddEffect(text, *textStyle, rect, *effectStyle);
    }
}

void UIManager::SetWorldUISCale(float scale)
{
    if (uiMopHPManager)
    {
        uiMopHPManager->ChangeZoomScale(scale);
    }
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
    Release();
}

void UIManager::SetRestartCallback(std::function<void()> cb)
{
    restartCallback = cb;
}

void UIManager::SetCurrentPlayer(Player* player)
{
    if (!player) return; 
    currentPlayer = player;

    if (uiStatusToolbar)
    {
        uiStatusToolbar->UpdateStat(player);
    }
}

Player* UIManager::GetCurrentPlayer()
{
    return currentPlayer;
}

void UIManager::DeleteLevelUI()
{
    UnregisterPlayer(currentPlayer);
    UnregisterEntity(currentPlayer);

    auto it = observerOwnerEntities.begin();
    while (it != observerOwnerEntities.end())
    {
        Entity* entity = *it;
        ++it;  
        UnregisterEntity(entity);
    }

    observerOwnerEntities.clear();

    for (auto uiCon_it = uiContainers.begin(); uiCon_it != uiContainers.end(); ++uiCon_it)
    {
        delete *uiCon_it;
        *uiCon_it = nullptr;
    }
    
    uiContainers.clear();

    delete uiEffectManager;
    uiEffectManager = nullptr;
    delete uiMopHPManager;
    uiMopHPManager = nullptr;
}

void UIManager::OnRelaseEntity(Entity* entity)
{
    UnregisterEntity(entity);
}

void UIManager::Init()
{
    tmMgr = TimerManager::GetInstance();
    rdt = D2DImage::GetRenderTarget();
    mouseManager = MouseManager::GetInstance();

    UIResourceSubManager::PreloadAll();
    
    /* 생성기 Load */
    uiMopHPManager = new UIMopHPManager();
    uiMopHPManager->Init();
    uiEffectManager = new UIEffectManager();

    uiGameOver = new UIGameOver();
    uiGameOver->Init(); 
    uiGameOver->SetActive(false);


    /* 기본 Load */
    uiStatusToolbar = new UIStatusToolbar();
    uiStatusToolbar->Init();

    uiQuickSlotToolbar = new UIQuickSlotToolbar();
    uiQuickSlotToolbar->Init();

    uiTopRightUI = new UITopRightUI();
    uiTopRightUI->Init();

    uiTextLogPanel = new UITextLogPanel();
    uiTextLogPanel->Init();

    uiStatusPanel = new UIStatusPanel();
    uiStatusPanel->Init();
    uiStatusPanel->SetActive(false);

    uiInventoryPanel = new UIInventory();
    uiInventoryPanel->Init();
    uiInventoryPanel->SetActive(false);


    /* UIContainers 버튼 연결 */
    uiQuickSlotToolbar->SetActionOnClick(0, [this](){});
    uiQuickSlotToolbar->SetActionOnClick(1, [this](){});
    uiQuickSlotToolbar->SetActionOnClick(2, [this]()
    {
        uiInventoryPanel->SetActive(!uiInventoryPanel->IsActive());
    });
    
    uiStatusToolbar->SetStatusButtonEvent([this]()
    {
        if (!uiStatusPanel->IsActive())
        {
            uiStatusPanel->UpdatePlayerStat(GetCurrentPlayer());
            uiStatusPanel->SetActive(true);
        }
        else
        {
            uiStatusPanel->SetActive(false);
        }
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
    /* Check Camera Zoom */
    if (CheckZoomChange())
    {
        SetWorldUISCale(zoomScale);
    }
    
    /* Mouse Click Check */
    if (MouseManager::GetInstance()->GetClickValueUsed() == false)
    {
        float mx = mouseManager->GetMousePos().x;
        float my = mouseManager->GetMousePos().y;

        if (uiGameOver && uiGameOver->HandleClick(mx, my))
        {
            MouseManager::GetInstance()->InitPoints();
            MouseManager::GetInstance()->AlreadyClickUsed();
        }
        else
        {
            for (auto uiContainer_it = uiContainers.rbegin(); uiContainer_it != uiContainers.rend(); ++uiContainer_it)
            {
                if ((*uiContainer_it)->HandleClick(mx, my))
                {
                    MouseManager::GetInstance()->InitPoints();
                    MouseManager::GetInstance()->AlreadyClickUsed();
                    break;
                }
            }
        }
    }
    /////////

    /* Update */
    float dt = tmMgr->GetDeltaTime();

    if (uiMopHPManager)
    {
        uiMopHPManager->Update(dt);
    }
    for (auto uiContainer_it = uiContainers.begin(); uiContainer_it != uiContainers.end(); ++uiContainer_it)
    {
        (*uiContainer_it)->Update(dt);
    }
    if (uiEffectManager)
    {
        uiEffectManager->Update(dt);
    }
    /*if (uiGameOver)
    {
        uiGameOver->Update(dt);
    }*/
    ///////


    if (KeyManager::GetInstance()->IsOnceKeyDown('K'))
    {
        currentPlayer->TakeDamage(3);
    }
    if (KeyManager::GetInstance()->IsOnceKeyDown('Z'))
    {
        DeleteLevelUI();
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
    if (uiMopHPManager)
    {
        uiMopHPManager->Render(rdt);
    }
    for (auto uiContainer_it = uiContainers.begin(); uiContainer_it != uiContainers.end(); ++uiContainer_it)
    {
        (*uiContainer_it)->Render(rdt);
    }
    if (uiEffectManager)
    {
        uiEffectManager->Render(rdt);
    }
    /*if (uiGameOver)
    {
        uiGameOver->Render(rdt);
    }*/
}

void UIManager::Release()
{
    //uiGameOver->SetActive(false);
    DeleteLevelUI();
    
    /* 절대 삭제 금지 */
    tmMgr = nullptr;
    rdt = nullptr;
    mouseManager = nullptr;
    camera = nullptr;
}

bool UIManager::CheckZoomChange()
{
    if (camera == nullptr) return false;

    if (camera->GetZoomScale() != zoomScale)
    {
        zoomScale = camera->GetZoomScale();
        return true;
    }

    return false;
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
