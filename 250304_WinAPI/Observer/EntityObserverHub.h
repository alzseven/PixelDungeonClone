#pragma  once
#include <vector>

namespace D2D1
{
    class ColorF;
}

class Entity;
struct EffectStyle;
struct TextStyle;
class IEntityObserver;
class ISystemObserver;
class Player;


// 공통 Entity 옵저버 허브
class EntityObserverHub {
private:
    std::vector<IEntityObserver*> observers;
    std::vector<ISystemObserver*> systemObservers;

public:
    void AddObserver(IEntityObserver* obs);
    void RemoveObserver(IEntityObserver* obs);
    void NotifyDamageTaken(Entity* e, int dmg, D2D1::ColorF color);
    void NotifyDeath(Entity* e);
    void NotifyChangePlayerInven(Player* p);
    
    void NotifySystemRelese(Entity* e);
};
