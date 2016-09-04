#include "stdafx.h"
#include "Image.h"
#include <malloc.h>

namespace ColoredConsole {
Image::Image(int sizeX, int sizeY) : Image::Image(sizeX, sizeY, {0x000000}) {

}

Image::Image(int sizeX, int sizeY, Color initialColor) : size_x_(sizeX), size_y_(sizeY) {
	buffer_ = static_cast<Color *>(malloc(size_x_ * size_y_ * sizeof(Color)));
	Clear(initialColor);
}

Image::~Image() {
	free(buffer_);
}

void Image::SetColor(int x, int y, Color color) const {
	buffer_[x + y * size_x_] = color;
}


Color Image::GetColor(int x, int y) const {
	return buffer_[x + size_x_ * y];
}

void Image::Clear() const {
	Clear({0x000000});
}

void Image::Clear(Color clearColor) const {
	for (auto x = 0; x < size_x_; x++) {
		for (auto y = 0; y < size_y_; y++) {
			SetColor(x, y, clearColor);
		}
	}
}
}
