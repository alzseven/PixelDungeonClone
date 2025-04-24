#pragma once
#include "UI/Text/UITextEffect.h"
#include <vector>

#include "../Observer/IEntityObserver.h"

class UIEffectManager : public IEntityObserver
{
private:
    std::vector<UITextEffect*> effects;

    TextStyle defaultTextStyle = {
        L"pixel", 16.0f, D2D1::ColorF(D2D1::ColorF::White), true,
         DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER
    };

    EffectStyle defaultEffectStyle =
    {
        2.0f, // duration
        -20.0f // y movespeed value
    };

public:
    ~UIEffectManager() override;

    void AddEffect(const std::wstring& text, const D2D1_RECT_F& rect)
    {
        auto* fx = new UITextEffect();
        fx->SetWorldRect(rect);
        fx->Init(text, defaultTextStyle, defaultEffectStyle);
        effects.push_back(fx);
    }

    void AddEffect(const std::wstring& text, const TextStyle& style, const D2D1_RECT_F& rect, const EffectStyle& effectStyle) {
        auto* fx = new UITextEffect();
        fx->SetWorldRect(rect);
        fx->Init(text, style, effectStyle);
        effects.push_back(fx);
    }

    void Update(float dt) {
        for (auto it = effects.begin(); it != effects.end(); ) {
            if ((*it)->IsDead()) {
                delete *it;
                it = effects.erase(it);
            }
            else {
                (*it)->Update(dt);
                ++it;
            }
        }
    }

    void Render(ID2D1HwndRenderTarget* rt) {
        for (auto* fx : effects) {
            if (fx)
                fx->Render(rt);
        }
    }

    void Clear() {
        for (auto* fx : effects) {
            delete fx;
        }
        effects.clear();
    }

    
    /* Observer */
    void OnEntityDamageTaken(DamageTakenData data) override
    {
        if (data.sendText == L"") return;

        if (!data.textStyle)
        {
            data.textStyle = &defaultTextStyle;
        }
        if (!data.effectStyle)
        {
            data.effectStyle = &defaultEffectStyle;
        }

        float halfWidth = 100.0f;
        float halfHight = 30.0f;
        // D2D1_RECT_F rect = {data.pos.x - halfWidth, data.pos.y - halfHight,
        //     data.pos.x + halfWidth, data.pos.y + halfHight};
        D2D1_RECT_F rect = {WINSIZE_X/2 - halfWidth, WINSIZE_Y/2 - halfHight,
            WINSIZE_X/2 + halfWidth, WINSIZE_Y/2 + halfHight};
        
        AddEffect(data.sendText, *data.textStyle, rect, *data.effectStyle);
    }
};

inline UIEffectManager::~UIEffectManager()
{
    for (auto* fx : effects) {
        delete fx;
    }
    effects.clear();
}
