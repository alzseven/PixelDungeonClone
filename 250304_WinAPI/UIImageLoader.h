#pragma once
#include "config.h"
#include "ImageManager.h"
#include "Image.h"

namespace UI
{
	const std::wstring BASE_RESOURCE_PATH = L"assets/interfaces/";
	constexpr int BASE_SCALE = 3;

	class UIImageLoader
	{
	public:
		UIImageLoader() = default;
		~UIImageLoader() = default;

		void Init()
		{
			_imgManager = ImageManager::GetInstance();

			if (_imgManager)
			{
				InitUIResource();
			}
		}
		void Release()
		{
			if (_imgManager)
			{
				UnloadUIResource();

				_imgManager = nullptr;
			}
		}

		void AddImage(const char* keyName, const wchar_t* filePath, int widht, int height, bool isTransparent = false, COLORREF color = RGB(0, 0, 0))
		{
			Image* img = _imgManager->AddImageAlpha(keyName, filePath, widht, height, isTransparent, color);
			if (img)
			{
				_mapImages.insert({ keyName, img });
			}
			_imgManager->DeleteImage(keyName);
		}

		void InitUIResource()
		{
			AddImage("chrome", (BASE_RESOURCE_PATH + L"chrome.png").c_str(), 128, 64, true, RGB(0, 0, 0));
			AddImage("status", (BASE_RESOURCE_PATH + L"status_pane.png").c_str(), 128, 128, true, RGB(0, 0, 0));

		} 

		void LoadUIResource()
		{
			_imgManager->FindImage("chrome");
		}

		void UnloadUIResource()
		{
			_imgManager->DeleteImage("");
		}

	private:
		ImageManager * _imgManager;
		map<string, Image*> _mapImages;

	};
}

