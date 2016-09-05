#pragma once
#include "Image.h"
#define IMAGE_FUNCTION(name, ...) \
	void name(Image * img, Color color, __VA_ARGS__)

namespace ColoredConsole {
	//TODO: Move to geometry
struct Vec2i {
	int x, y;
};
//TODO: Move to geometry
struct Vec3f {
	float x, y, z;
};
//TODO: Move to geometry
struct Triangle {
	Vec2i v_0, v_1, v_2;
};
//TODO: Move to geometry
struct Rectangle {
	Vec2i top_left, bottom_right;
};

namespace ImageHelper {
	

	WINCOLOREDCONSOLE_API IMAGE_FUNCTION(DrawLine, int x_0, int y_0, int x_1, int y_1);
	WINCOLOREDCONSOLE_API IMAGE_FUNCTION(DrawLine, Vec2i p_0, Vec2i p_1);
	WINCOLOREDCONSOLE_API IMAGE_FUNCTION(DrawTriangle, Triangle t);
	WINCOLOREDCONSOLE_API IMAGE_FUNCTION(DrawCircle, Vec2i center, int radius);
};
}


