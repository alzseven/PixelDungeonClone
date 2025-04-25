#pragma once
#include "UIValueBar.h"
#include "../../UIManager.h"
#include "../VisualStyle.h"
#include "../Core/UIContainerBase.h"
#include "../../Observer/IEntityObserver.h"
#include "../Util/UIHelper.h"

class UIMopBar : public UIContainerBase, public IEntityObserver {
protected:
    ImageStyle bgStyle;
    BarStyle barStyle;
    
    UIValueBar* bar{nullptr};

    Camera* camera{nullptr};
    Entity* target{nullptr};

    D2D1_SIZE_F offset = {0.f, -30.f};

public:
    ~UIMopBar() override;
    
    void OnEntityDamageTaken(DamageTakenData data) override;
    void OnEntityDied(Entity* entity) override;
    
    void Init(const D2D1_RECT_F& layout = {0,0,49,8},
        const ImageStyle& bs = {nullptr},
        const BarStyle& s = {{nullptr},{nullptr}},
        float max = 10.0f);
    void SetStyle(const ImageStyle& bgs, const BarStyle& s);
    void SetValue(float value);
    void SetMaxValue(float max);
    void SetTarget(Entity* target, Camera* cam);

    void Update(float deltaTime) override;
    void Render(ID2D1HwndRenderTarget* renderTarget) override;

protected:
    void ChaseTargetPos();
};
