#pragma once

namespace UI
{
	class IButton 
	{
	public:
		virtual void OnClick() = 0;
		virtual void UpdateState() = 0;
	};

}