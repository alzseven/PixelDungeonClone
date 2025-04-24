#pragma once
#include "Entity.h"

class Level;
class PathFinder;
class Item;
class Inventory;
class Player : public Entity
{
public:
    using Super = Entity;
    Player(FPOINT pos, float speed, int hp, int attDmg, int defense);
    virtual ~Player();

    void Update() override;
    void Render(HDC hdc) override;
    virtual void Act(Level* level) override;
    virtual void Attack(Level* level) override;
    void Heal(int healAmount) override;
    void SetState(EntityState state) override;
    
    void Move(Level* level);
    void ActIdle(Level* level);

    bool NeedsInput() override { return true; }
    bool IsBusy() override { return curState != EntityState::IDLE; }

    void SetNextPos(FPOINT pos) { destPos = pos; }
    void GetItem(Item* item);
    inline Inventory* GetInven() { return inven; }
private:
    Inventory* inven;
};
