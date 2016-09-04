#pragma once

#ifdef WINCONSOLERENDERER_EXPORTS
#define WINCONSOLERENDERER_API __declspec(dllexport) 
#else
#define WINCONSOLERENDERER_API __declspec(dllimport) 
#endif

#include "IRenderer.h"
#include <windows.h>

namespace WinConsoleRenderer {
	class WinConsoleRenderer :
		public IRenderer
	{
	public:
		WINCONSOLERENDERER_API WinConsoleRenderer(int sizeX, int sizeY);
		~WinConsoleRenderer();

		virtual void WINCONSOLERENDERER_API render(const Image * img);

		void TestColor();

	private:
		int _sizeX, _sizeY;
		WORD _baseAtr;
		HANDLE  _hConsole;
	};
}
