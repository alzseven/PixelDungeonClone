#pragma once
#include "UI/Button/UIImageTextButton.h"
#include "UI/Text/UITextBox.h"
#include "UI/Util/UIResourceSubManager.h"
#include "Observer/IEntityObserver.h"
#include "UI/Core/UIContainer.h"
#include "UI/Bar/UIValueBar.h"
#include "UI/Util/UIHelper.h"
#include "UI/VisualStyle.h"

class UIGameOver : public UIContainer, public IEntityObserver {
private:
    UIImage* background;
    UIImage* banner;
    UIImageTextButton* retryButton;
    UIImageTextButton* gameFinishButton;

public:

    void SetRestartCallBack(function<void()> cb) {
        if (retryButton) {
            retryButton->SetOnClick(cb);
        }
    }

    void SetExitCallBack(function<void()> cb) {
        if (gameFinishButton) {
            gameFinishButton->SetOnClick(cb);
        }
    }

    void Init(const D2D1_RECT_F& rect = { 0.0f, 0.0f, WINSIZE_X, WINSIZE_Y }) override
    {
        SetRect(rect);
        //UIResourceSubManager::Preload_StatusToolbar();
       // 반투명 배경 추가
       auto bgSource = D2DImageManager::GetInstance()->AddImage("GameOverBg", L"Image/GameOverBg.png");
       // 게임 오버 텍스트 이미지 추가
       auto bannerSource = D2DImageManager::GetInstance()->AddImage("GameOverBanner", L"Image/GameOverBanner2.png");

        ImageStyle bgStyle;
        bgStyle.image = bgSource;

        ImageStyle bannerStyle;
        bannerStyle.image = bannerSource;

        NinePatchStyle PanelStyle = UIHelper::CreateNinePatchFromSheet("chrome", "chrome", { 6.0f, 6.0f });

        UIButtonStyle retryButtonStyle;
        retryButtonStyle.background = { nullptr };
        retryButtonStyle.textStyle = { {} };
        retryButtonStyle.textStyle.fontSize = 25.0f;
        //retryButtonStyle.textStyle.bold = true;
        retryButtonStyle.textStyle.horizontalAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
        retryButtonStyle.textStyle.verticalAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

        UIButtonStyle finishButtonStyle;
        finishButtonStyle.background = { nullptr };
        finishButtonStyle.textStyle = { {} };
        finishButtonStyle.textStyle.fontSize = 25.0f;
        //finishButtonStyle.textStyle.bold = true;
        finishButtonStyle.textStyle.horizontalAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
        finishButtonStyle.textStyle.verticalAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;


        background = UIHelper::ApplyImageStyle(this, GetSizeRect(), bgStyle);
        banner = UIHelper::ApplyImageStyle(this, {356, 170, 356+368, 170+139}, bannerStyle);

        auto buttonBg1 = UIHelper::ApplyNinePathStyle(this, { 434, 437, 434 + 211, 437 + 56 }, PanelStyle);
        retryButton = UIHelper::ApplyButtonStyle(this, { 434, 437, 434 + 211, 437 + 56 }, retryButtonStyle);
        UIHelper::SetButtonText(*retryButton, L"Retry", 0);

        auto buttonBg2 = UIHelper::ApplyNinePathStyle(this, { 434, 525, 434 + 211, 525 + 56 }, PanelStyle);
        gameFinishButton = UIHelper::ApplyButtonStyle(this, {434, 525, 434+211, 525+56}, finishButtonStyle);
        UIHelper::SetButtonText(*gameFinishButton, L"Exit", 0);
       /* background->SetActive(false);
        banner->SetActive(false);*/

    }

    void OnEntityDied(Entity* entity) override {
        ///게임 오버 화면 나오게 하기
        SetActive(true);
        ///다시 시작, 게임 종료 버튼 나오게 하기
        ///마우스 입력 받기
    }
    
}; 
