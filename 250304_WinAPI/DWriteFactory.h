#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <wrl.h>
#include <map>
#include <string>

class DWriteFactory {
private:
    Microsoft::WRL::ComPtr<IDWriteFactory> factory;
    std::map<std::pair<std::wstring, float>, Microsoft::WRL::ComPtr<IDWriteTextFormat>> fontCache;

    // Singleton
    DWriteFactory() { Init(); }
    void Init();

public:
    static DWriteFactory* GetInstance() {
        static DWriteFactory instance;
        return &instance;
    }

    void AddFont(const std::wstring& name, float size);
    IDWriteTextFormat* FindFormat(const std::wstring& name, float size);
    bool CreateTextFormat(
        const std::wstring& fontName,
        IDWriteTextFormat** outFormat,
        float fontSize,
        bool bold = false,
        DWRITE_TEXT_ALIGNMENT horizontalAlign = DWRITE_TEXT_ALIGNMENT_LEADING,
        DWRITE_PARAGRAPH_ALIGNMENT verticalAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR
    );
    void CreateTextLayout(const std::wstring& text, IDWriteTextFormat* format, float width, float height, IDWriteTextLayout** layout);
};
