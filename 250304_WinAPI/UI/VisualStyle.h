#pragma once
#include <array>
#include <string>
#include "../D2DImage.h"

struct D2DFrameImage;

struct ImageStyle {
    D2DImage* image = nullptr;
    FRECT padding = {0,0,0,0};
    float alpha = 1.0f;
    D2D1_COLOR_F tint = D2D1::ColorF(1, 1, 1, 1); // 색상 효과
    bool grayscale = false; // 비활성화 상태 표현
    int frameIdx;
    int frameIdy;
};

struct BarStyle {
    ImageStyle background;
    ImageStyle fill;
    ImageStyle handle;
};

struct TextStyle {
    std::wstring fontName = L"맑은 고딕";
    float fontSize = 16.0f;
    D2D1_COLOR_F color = D2D1::ColorF(1, 1, 1, 1); // 기본 하양
    bool bold = false;
    DWRITE_TEXT_ALIGNMENT horizontalAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
    DWRITE_PARAGRAPH_ALIGNMENT verticalAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

    TextStyle() = default;
};

struct EffectStyle {
    float lifetime = 2.0f;
    float moveSpeed = -20.0f;

    EffectStyle() = default;
};

struct UIIconStyle {
    ImageStyle bgStyle{nullptr};
    ImageStyle iconStyle{nullptr};
};

struct UIInventorySlotStyle {
    ImageStyle background;         // 슬롯 배경
    ImageStyle itemIcon;          // 아이템 아이콘 이미지
    TextStyle quantityTextStyle;  // 좌상단 수량 텍스트
    TextStyle enhancementTextStyle; // 우하단 강화 수치 텍스트
};

struct UIButtonStyle {
    ImageStyle background;
    TextStyle textStyle;
};

// UI9PatchPanel 전용 구조는 분리
// 9개 영역 중 하나에 대한 렌더링 정보
struct NinePatchRegion {
    D2D1_RECT_F srcRect = {0, 0, 0, 0}; // 이미지 내 잘라낼 부분
    float alpha = 1.0f;                 // 투명도 (1.0 = 불투명)
    D2D1_COLOR_F tint = D2D1::ColorF(1, 1, 1, 1); // 색상 필터
};

// 9패치 전체 스타일 정의
struct NinePatchStyle {
    D2DImage* image = nullptr; // 단일 이미지 기반 (프레임 불필요)
    std::array<NinePatchRegion, 9> regions; // 순서: TL, T, TR, L, C, R, BL, B, BR
    D2D1_SIZE_F cornerSize = {8.0f, 8.0f};   // 모서리 크기

    // region 접근 편의 함수
    NinePatchRegion& TopLeft()     { return regions[0]; }
    NinePatchRegion& Top()         { return regions[1]; }
    NinePatchRegion& TopRight()    { return regions[2]; }
    NinePatchRegion& Left()        { return regions[3]; }
    NinePatchRegion& Center()      { return regions[4]; }
    NinePatchRegion& Right()       { return regions[5]; }
    NinePatchRegion& BottomLeft()  { return regions[6]; }
    NinePatchRegion& Bottom()      { return regions[7]; }
    NinePatchRegion& BottomRight() { return regions[8]; }
};