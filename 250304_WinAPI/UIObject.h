#pragma once
#include "config.h"

namespace UI
{
	struct ImageData
	{
		const char* keyName;
		const wchar_t* filePath;
		bool isTransparent{ false };
		COLORREF transColor{ RGB(0,0,0) };
	};

	class UIObject
	{
	public:
		UIObject() = default;
		virtual ~UIObject() = default;

		virtual void Init(RECT rect);
		virtual void Init(int dx = 0, int dy = 0, int width = 0, int height = 0);
		virtual void Release() = 0;
		virtual void Update() = 0;
		virtual void Render(HDC hdc) = 0;

		void SetPos(int dx, int dy);
		void SetScale(int width, int height);

	protected:
		void UpdateRectTransform();

	protected:
		int centerX{ 0 };
		int centerY{ 0 };
		int width{ 0 };
		int height{ 0 };
		RECT rectTransform{ 0, 0, 0, 0 };
	};

}