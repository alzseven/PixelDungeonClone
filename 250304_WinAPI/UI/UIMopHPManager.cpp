#pragma once
#include "UIMopHPManager.h"

#include "../Entity.h"
#include "Bar/UIMopBar.h"
#include "Util/UIHelper.h"


UIMopHPManager::~UIMopHPManager()
{
    Release();
}

void UIMopHPManager::Init()
{
    SetDefaultStyle();
}
void UIMopHPManager::Update(float dt)
{
    for (auto& [entity, bar] : uiMopHPBars)
    {
        if (entity && bar)
            bar->Update(dt);
    }
}
    
void UIMopHPManager::Render(ID2D1HwndRenderTarget* rt) {
    std::wcout << L"맵 사이즈: " << uiMopHPBars.size() << std::endl;
    for (auto& [entity, bar] : uiMopHPBars)
    {
        if (entity && bar)
            bar->Render(rt);
    }
}

void UIMopHPManager::Release()
{
    auto it = uiMopHPBars.begin();
    while (it != uiMopHPBars.end())
    {
        Entity* entity = it->first;
        UIMopBar* bar = it->second;

        if (entity && bar)
        {
            entity->GetEntityObserverHub().RemoveObserver((IEntityObserver*)bar);
            delete bar;
        }

        it = uiMopHPBars.erase(it);  // ✅ erase 후, 다음 반복자 리턴
    }
}

UIMopBar* UIMopHPManager::CreateMopHPBar(Entity* entity, Camera* camera)
{
    if (!entity || !camera ) return nullptr;
    if (entity->GetType() != EntityType::MONSTER) return nullptr;
    
    auto hpBar = new UIMopBar();
    auto entityPos = entity->GetPosition();
    auto entityRect = {entity};
    hpBar->Init();
    hpBar->SetStyle(defulatMopHPBarBgStyle, defaultMopHPBarStyle);
    hpBar->SetMaxValue(entity->GetMaxHP());
    hpBar->SetValue(entity->GetHP());
    hpBar->SetTarget(entity, camera);
    hpBar->SetActive(false);
    uiMopHPBars.insert({entity, hpBar});
    return hpBar;
}

UIMopBar* UIMopHPManager::FindMopBar(Entity* entity)
{
    auto it = uiMopHPBars.find(entity);
    if (it != uiMopHPBars.end())
    {
        return it->second;
    }
    return nullptr;
}

bool UIMopHPManager::RemoveMopBar(Entity* entity)
{
    auto it = uiMopHPBars.find(entity);
    if (it != uiMopHPBars.end())
    {
        if (!it->second) return true;
        delete it->second;
        it->second = nullptr;
        it = uiMopHPBars.erase(it);
        return true;
    }
    return false;
}

void UIMopHPManager::DetachMopBar(Entity* entity)
{
    if (!entity) return;

    UIMopBar* bar = FindMopBar(entity);
    if (bar)
    {
        entity->GetEntityObserverHub().RemoveObserver((IEntityObserver*)bar);
    }
    RemoveMopBar(entity);
}

void UIMopHPManager::ChangeZoomScale(float zoomScale)
{
    for (auto& [entity, bar] : uiMopHPBars)
    {
        if (bar)
            bar->SetScale({zoomScale, zoomScale});
    }
}

void UIMopHPManager::SetDefaultStyle()
{
    defulatMopHPBarBgStyle = ImageStyle
    {
        D2DImageManager::GetInstance()->FindImage(("mop_hpbar_bg")),
        {0,0,0,0},
        1.0f
    };
    defaultMopHPBarStyle = BarStyle
    {
        ImageStyle {D2DImageManager::GetInstance()->FindImage("mop_hpbar_fill")},
        ImageStyle{ nullptr }
    };
}
