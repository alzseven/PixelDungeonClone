#include "UITextBox.h"  
#include "UIText.h"  
#include "ImageManager.h"  
#include "Image.h"

using namespace UI;  

void UITextBox::Init(RECT rect, const string& text, ImageData bgData, RECT margin)
{  
   UIObject::Init(rect);  

   ResourceInit(text, bgData, margin);
}  

void UITextBox::Init(int dx, int dy, int width, int height, const string& text, ImageData bgData, RECT margin)
{  
   UIObject::Init(dx, dy, width, height);  

   ResourceInit(text, bgData, margin); 
}  

void UITextBox::Release()  
{  
   if (textUI)  
   {  
       textUI->Release();  
       delete textUI;  
       textUI = nullptr;  
   }  

   if (bg)  
   {  
       bg = nullptr;  
   }  
}  

void UITextBox::Update()  
{  
}  

void UITextBox::Render(HDC hdc)  
{  
    if (bg)
    {
        bg->Render(hdc, rectTransform.left, rectTransform.top);
    }

   if (textUI)  
   {  
       textUI->Render(hdc);  
   }  
}  

void UITextBox::SetText(const string& text)  
{  
   if (textUI)  
   {  
       textUI->SetText(text);  
   }  
}  

void UITextBox::ResourceInit(const string& text, ImageData bgData, RECT margin)
{  
    if (bgData.keyName != "")
    {
        bg = ImageManager::GetInstance()->AddImage(bgData.keyName, bgData.filePath, width, height);
    }
   textUI = new UIText();  
   textUI->Init(rectTransform, text);  
   textUI->SetPos(centerX + margin.left, centerY + margin.top);
}
