#include "EntityObserverHub.h"
#include "IEntityObserver.h"
#include "../UI/VisualStyle.h"
#include "../Entity.h"
#include "../Player.h"


void EntityObserverHub::AddObserver(IEntityObserver* obs) {
    observers.push_back(obs);
}

void EntityObserverHub::RemoveObserver(IEntityObserver* obs) {
    observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
}

void EntityObserverHub::NotifyDamageTaken(Entity* e, int dmg, D2D1::ColorF color) {
    if (!e) return;

    DamageTakenData data;
    data.entity = e;
    data.sendText = to_wstring(dmg);
    data.pos = e->GetPosition();
    data.textStyle = new TextStyle;
    data.textStyle->color = color;
    data.textStyle->horizontalAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
    data.textStyle->verticalAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
    data.effectStyle = new EffectStyle;
        
    for (auto* obs : observers)
    {
        obs->OnEntityDamageTaken(data);
    }
}

void EntityObserverHub::NotifyDeath(Entity* e) {
    for (auto* obs : observers) obs->OnEntityDied(e);
}

void EntityObserverHub::NotifyChangePlayerInven(Player* p)
{
    for (auto* obs : observers) obs->OnChangePlayerInven(p);
}
