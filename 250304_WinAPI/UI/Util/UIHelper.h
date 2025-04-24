#pragma once
#include <d2d1.h>
#include <functional>
#include <string>

#include "../Core/UIContainerBase.h"


class UI9PatchImage;
class UIImage;
class UIText;
struct TextStyle;
struct ImageStyle;
struct UIButtonStyle;
struct UIIconStyle;
struct UIInventorySlotData;
struct UIInventorySlotStyle;
class UIImageTextButton;
class UIContainerBase;
struct NinePatchStyle;


namespace UIHelper {
    /* 생성 코드 */
    UI9PatchImage* ApplyNinePathStyle(UIContainerBase* target, const D2D1_RECT_F& localRect, const NinePatchStyle& style);
    UIText* ApplyTextStyle(UIContainerBase* target, const D2D1_RECT_F& localRect, const TextStyle& style);
    UIImage* ApplyImageStyle(UIContainerBase* target, const D2D1_RECT_F& localRect, const ImageStyle& style);
    UIImageTextButton* ApplyInventorySlotStyle(UIContainerBase* target, const D2D1_RECT_F& localRect, const UIInventorySlotStyle& style,
        const std::function<void()>& onClick = nullptr, bool clone = false);
    UIImageTextButton* ApplyIconStyle(UIContainerBase* target, const D2D1_RECT_F& localRect, const UIIconStyle& style,
        const std::function<void()>& onClick = nullptr, bool clone = false);
    UIImageTextButton* ApplyButtonStyle(UIContainerBase* target, const D2D1_RECT_F& localRect, const UIButtonStyle& style,
        const std::function<void()>& onClick = nullptr, bool clone = false);
    UIImageTextButton* ApplyNinePatchButtonStyle(UIContainerBase* target, const D2D1_RECT_F& localRect, const UIButtonStyle& style,
    const NinePatchStyle& patchStyle, const std::function<void()>& onClick = nullptr, bool clone = false );

    NinePatchStyle CreateNinePatchFromSheet(const std::string& srcKey, const std::string& targetKey, const D2D1_SIZE_F& cornerSize);
    
    /* 범용 값 변경 코드 */
    void SetButtonText(const UIImageTextButton& btn, const std::wstring& text, size_t index = 0);
    void SetButtonImage(const UIImageTextButton& btn, const ImageStyle& style, size_t index = 0);
    void SetButtonImage(const UIImageTextButton& btn, const NinePatchStyle& style, size_t index = 0);
    /* 인벤 전용 */
    void SetInventorySlotData(const UIImageTextButton& target, const UIInventorySlotData* data);
    void UpdateInventorySlot(UIImageTextButton& slot,
                             const UIInventorySlotData& data, const std::function<void()>& onClick = nullptr);
    

    /* 공통 템플릿 및 수학용 */
    float Clamp(float value, float minVal, float maxVal);
    
    template <typename T, typename... Args>
    T* MakeUI(Args&&... args) {
        return new T(std::forward<Args>(args)...);
    }

}
