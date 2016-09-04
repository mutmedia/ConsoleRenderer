#include "Image.h"
#include <stdlib.h>
#include <malloc.h>

namespace WinConsoleRenderer {
	Image::Image(int sizeX, int sizeY) : Image::Image(sizeX, sizeY, { 0x000000 }) {

	}

	Image::Image(int sizeX, int sizeY, Color initialColor) : _sizeX(sizeX), _sizeY(sizeY) {
		_buffer = (Color *)malloc(_sizeX*_sizeY*sizeof(Color));
		clear(initialColor);
	}

	Image::~Image() {
		free(_buffer);
	}

	void Image::setColor(int x, int y, Color color)
	{
		_buffer[x + y * _sizeX] = color;
	}


	Color Image::getColor(int x, int y) const
	{
		return _buffer[x + _sizeX  *y];
	}

	void Image::clear() {
		clear({ 0x000000 });
	}

	void Image::clear(Color clearColor) {
		for (auto x = 0; x < _sizeX; x++) {
			for (auto y = 0; y < _sizeY; y++) {
				setColor(x, y, clearColor);
			}
		}
	}
}
