#pragma once
#include "Image.h"
#include "Geometry.h"

#define IMAGE_FUNCTION(name, ...) \
	void name(Image * img, Color color, __VA_ARGS__)

using namespace Geometry;

namespace ColoredConsole {
namespace ImageHelper {
	WINCOLOREDCONSOLE_API IMAGE_FUNCTION(DrawLine, int x_0, int y_0, int x_1, int y_1);
	WINCOLOREDCONSOLE_API IMAGE_FUNCTION(DrawLine, Vec2i p_0, Vec2i p_1);
	WINCOLOREDCONSOLE_API IMAGE_FUNCTION(DrawTriangle, Triangle t);
	WINCOLOREDCONSOLE_API IMAGE_FUNCTION(DrawCircle, Vec2i center, int radius);
};
}


