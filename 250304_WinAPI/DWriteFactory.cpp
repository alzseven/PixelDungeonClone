#include "DWriteFactory.h"
#include <cassert>
#include "config.h"

void DWriteFactory::Init() {
    HRESULT hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(factory.GetAddressOf())
    );
    assert(SUCCEEDED(hr) && "DWriteFactory 초기화 실패");
}

void DWriteFactory::AddFont(const std::wstring& name, float size) {
    Microsoft::WRL::ComPtr<IDWriteTextFormat> format;
    factory->CreateTextFormat(
        name.c_str(), nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        size,
        L"", // locale
        &format
    );
    fontCache[{name, size}] = format;
}

IDWriteTextFormat* DWriteFactory::FindFormat(const std::wstring& name, float size) {
    auto it = fontCache.find({ name, size });
    if (it != fontCache.end()) {
        return it->second.Get();
    }
    return nullptr;
}

bool DWriteFactory::CreateTextFormat(
    const std::wstring& fontName,
    IDWriteTextFormat** outFormat,
    float fontSize,
    bool bold,
    DWRITE_TEXT_ALIGNMENT hAlign,
    DWRITE_PARAGRAPH_ALIGNMENT vAlign
) {
    if (!factory) return false;

    DWRITE_FONT_WEIGHT weight = bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL;

    HRESULT hr = factory->CreateTextFormat(
        fontName.c_str(),
        nullptr,
        weight,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        L"ko-kr",
        outFormat
    );

    if (FAILED(hr) || !*outFormat) return false;

    (*outFormat)->SetTextAlignment(hAlign);
    (*outFormat)->SetParagraphAlignment(vAlign);

    return true;
}

void DWriteFactory::CreateTextLayout(const std::wstring& text, IDWriteTextFormat* format, float width,
    float height, IDWriteTextLayout** layout)
{
    factory->CreateTextLayout(
        text.c_str(),
        static_cast<UINT32>(text.length()),
        format,
        width,
        height,
        layout);
}
