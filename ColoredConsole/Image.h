#pragma once

#ifdef WINCOLOREDCONSOLE_EXPORTS
#define WINCOLOREDCONSOLE_API __declspec(dllexport) 
#else
#define WINCOLOREDCONSOLE_API __declspec(dllimport) 
#endif

namespace ColoredConsole {
	union Color {
		unsigned int val;
		struct {
			unsigned char b, g, r, a;
		};
	};

	class Image
	{
	public:
		WINCOLOREDCONSOLE_API Image(int sizeX, int sizeY);
		WINCOLOREDCONSOLE_API Image(int sizeX, int sizeY, Color initialC);
		~Image();
		void WINCOLOREDCONSOLE_API SetColor(int x, int y, Color color) const;
		Color WINCOLOREDCONSOLE_API GetColor(int x, int y) const;
		void WINCOLOREDCONSOLE_API Clear() const;
		void WINCOLOREDCONSOLE_API Clear(Color clearColor) const;
		int width() const { return size_x_; }
		int height() const { return size_y_; }
		float *zbuffer;		

	private:
		int size_x_, size_y_;
		Color * buffer_;
		
	};
}

