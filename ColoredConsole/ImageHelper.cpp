#include "stdafx.h"
#include "ImageHelper.h"
#include <memory>
#include <vector> 

namespace ColoredConsole {
namespace ImageHelper {

//TODO: Move to geometry
Vec3f cross(Vec3f u, Vec3f v) {
	return{ u.y * v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x };
}

IMAGE_FUNCTION(DrawLine, int x_0, int y_0, int x_1, int y_1)
{
	bool steep = false;
	if (std::abs(x_0 - x_1)<std::abs(y_0 - y_1)) {
		std::swap(x_0, y_0);
		std::swap(x_1, y_1);
		steep = true;
	}
	if (x_0>x_1) {
		std::swap(x_0, x_1);
		std::swap(y_0, y_1);
	}
	int dx = x_1 - x_0;
	int dy = y_1 - y_0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y_0;
	for (int x = x_0; x <= x_1; x++) {
		if (steep) {
			img->SetColor(y, x, color);
		}
		else {
			img->SetColor(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y_1>y_0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

IMAGE_FUNCTION(DrawLine, Vec2i p_0, Vec2i p_1)
{
	DrawLine(img, color, p_0.x, p_0.y, p_1.x, p_1.y);
}

IMAGE_FUNCTION(DrawTriangle, Triangle t)
{
	Rectangle bb = {};
	bb.bottom_right = { 0, 0 };
	bb.top_left = { img->width() - 1, img->height() - 1 };
	
	bb.top_left.x = max(0, min(bb.top_left.x, t.v_0.x));
	bb.top_left.y = max(0, min(bb.top_left.y, t.v_0.y));
	bb.top_left.x = max(0, min(bb.top_left.x, t.v_1.x));
	bb.top_left.y = max(0, min(bb.top_left.y, t.v_1.y));
	bb.top_left.x = max(0, min(bb.top_left.x, t.v_2.x));
	bb.top_left.y = max(0, min(bb.top_left.y, t.v_2.y));

	bb.bottom_right.x = min(img->width(),	max(bb.bottom_right.x, t.v_0.x));
	bb.bottom_right.y = min(img->height(),	max(bb.bottom_right.y, t.v_0.y));
	bb.bottom_right.x = min(img->width(),	max(bb.bottom_right.x, t.v_1.x));
	bb.bottom_right.y = min(img->height(),	max(bb.bottom_right.y, t.v_1.y));
	bb.bottom_right.x = min(img->width(),	max(bb.bottom_right.x, t.v_2.x));
	bb.bottom_right.y = min(img->height(),	max(bb.bottom_right.y, t.v_2.y));

	auto IsInsideTriangle = [](Vec2i point, Triangle triangle)
	{
		// use barycentricCoords
		auto u = cross({ float(triangle.v_2.x - triangle.v_0.x), float(triangle.v_1.x - triangle.v_0.x), float(triangle.v_0.x - point.x) },
		{ float(triangle.v_2.y - triangle.v_0.y), float(triangle.v_1.y - triangle.v_0.y), float(triangle.v_0.y - point.y) });
		Vec3f bar = (std::abs(u.z) < 1) ? Vec3f{ -1, 1, 1 } :
			Vec3f{ 1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z };
		return !(bar.x < 0 || bar.y < 0 || bar.z < 0);
	};

	for (int x = bb.top_left.x; x <= bb.bottom_right.x; x++) {
		for (int y = bb.top_left.y; y <= bb.bottom_right.y; y++) {
			if (IsInsideTriangle(Vec2i{ x, y }, t)) {
				img->SetColor(x, y, color);
			}
		}
	}

}
}

IMAGE_FUNCTION(DrawCircle, Vec2i center, int radius)
{
	auto x = radius;
	auto y = 0;
	auto err = 0;

	// Bresenham Circle Drawing Algorithm
	while (x >= y)
	{
		img->SetColor(center.x + x, center.y + y, color);
		img->SetColor(center.x + y, center.y + x, color);
		img->SetColor(center.x - y, center.y + x, color);
		img->SetColor(center.x - x, center.y + y, color);
		img->SetColor(center.x - x, center.y - y, color);
		img->SetColor(center.x - y, center.y - x, color);
		img->SetColor(center.x + y, center.y - x, color);
		img->SetColor(center.x + x, center.y - y, color);

		y += 1;
		err += 1 + 2 * y;
		if (2 * (err - x) + 1 > 0)
		{
			x -= 1;
			err += 1 - 2 * x;
		}
	}
}
}
