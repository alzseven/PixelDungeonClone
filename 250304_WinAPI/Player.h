#pragma once
#include "Entity.h"

class Level;
class PathFinder;
class Item;
class Inventory;
class Player : public Entity
{
private:
    FPOINT nowAscPos;
    FPOINT nowDescPos;
    bool justMoved{ true };

    function<void()> Ascending;
    function<void()> Descending;
public:
    using Super = Entity;
    Player(FPOINT pos, float speed, int hp, int attDmg, int defense);
    virtual ~Player();

    void SetStairs(const FPOINT& ascPos, const FPOINT& descPos);
    inline void SetJustMoved(bool moved) { justMoved = moved; }

    void Update() override;

    void Render(HDC hdc) override;
    virtual void Act(Level* level) override;
    virtual void Attack(Level* level) override;
    void Heal(int healAmount) override;
    void UseItem(Level* level);

    void SetState(EntityState state) override;
    
    void Move(Level* level);
    void ActIdle(Level* level);

    bool NeedsInput() override { return true; }
    bool IsBusy() override { return curState != EntityState::IDLE; }

    
    void SetNextPos(FPOINT pos) { destPos = pos; }
    void GetItem(Item* item);
    inline Inventory* GetInven() { return inven; }

    void SetFunctions(function<void()> asc, function<void()> desc);
private:
    Inventory* inven;
};
