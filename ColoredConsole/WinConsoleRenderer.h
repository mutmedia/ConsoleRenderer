#pragma once

#ifdef WINCOLOREDCONSOLE_EXPORTS
#define WINCOLOREDCONSOLE_API __declspec(dllexport) 
#else
#define WINCOLOREDCONSOLE_API __declspec(dllimport) 
#endif

#include "IRenderer.h"
#include <windows.h>
#include <unordered_map>

namespace ColoredConsole {
	class WinConsoleRenderer :
		public IRenderer
	{
	public:
		WINCOLOREDCONSOLE_API WinConsoleRenderer(int sizeX, int sizeY);
		~WinConsoleRenderer();
		void WINCOLOREDCONSOLE_API Render(const Image * img) override;
		virtual void WINCOLOREDCONSOLE_API ShowFps(const Image * img);		
		void WINCOLOREDCONSOLE_API TestColor();
		float fps() const { return fps_; };

	private:
		const int kForeShading[5] = { 0x00, 0xb0, 0xb1, 0xb2, 0xdb };

		short size_x_, size_y_;

		HANDLE  console_handle_;
		WORD base_atr_;		
		
		CHAR_INFO * char_info_array_;
		std::unordered_map<unsigned int, CHAR_INFO> rgb_to_char_info_buffer_;
		COORD char_info_array_size_;
		SMALL_RECT buffer_rectangle_;

		float fps_;

		CHAR_INFO ColorCodeToCharInfo(int color_code) const;
		static int RgbToColorCode(Color color);
		CHAR_INFO RgbToCharInfo(Color color) const;
	};
}
