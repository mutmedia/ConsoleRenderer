#pragma once

#ifdef WINCONSOLERENDERER_EXPORTS
#define WINCONSOLERENDERER_API __declspec(dllexport) 
#else
#define WINCONSOLERENDERER_API __declspec(dllimport) 
#endif

namespace WinConsoleRenderer {
	union Color {
		unsigned int val;
		struct {
			unsigned char b, g, r, a;
		};
	};

	class Image
	{
	public:
		WINCONSOLERENDERER_API Image(int sizeX, int sizeY);
		WINCONSOLERENDERER_API Image(int sizeX, int sizeY, Color initialC);
		~Image();
		void WINCONSOLERENDERER_API setColor(int x, int y, Color color);
		Color WINCONSOLERENDERER_API getColor(int x, int y) const;
		void WINCONSOLERENDERER_API clear();
		void WINCONSOLERENDERER_API clear(Color clearColor);

	private:
		int _sizeX, _sizeY;
		Color * _buffer;
	};
}

