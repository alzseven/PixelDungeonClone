#pragma once
#include "VisualStyle.h"

class Entity;
class UIMopBar;
class Camera;

class UIMopHPManager
{
protected:
    unordered_map<Entity*, UIMopBar*> uiMopHPBars;

    ImageStyle defulatMopHPBarBgStyle;
    BarStyle defaultMopHPBarStyle;
    
public:
    UIMopHPManager() = default;
    ~UIMopHPManager();

    void Init();
    void Update(float dt);
    void Render(ID2D1HwndRenderTarget* rt);
    void Release();
    UIMopBar* CreateMopHPBar(Entity* entity, Camera* camera);
    UIMopBar* FindMopBar(Entity* entity);
    bool RemoveMopBar(Entity* entity);
    void DetachMopBar(Entity* entity);
    void ChangeZoomScale(float zoomScale);
    void SetDefaultStyle();
};
