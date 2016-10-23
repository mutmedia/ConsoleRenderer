// ConsoleRendererClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinConsoleRenderer.h"
#include "Image.h"
#include "ImageHelper.h"
#include "Geometry.h"
#include <vector>
#include <time.h>



using namespace ColoredConsole;
using namespace Geometry;

const Vec3f CubeTris[36] = {
	{ -1.0f,-1.0f,-1.0f },
	{ -1.0f,-1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f },
	{ -1.0f,-1.0f,-1.0f },
	{ -1.0f, 1.0f,-1.0f },
	{ 1.0f,-1.0f, 1.0f },
	{ -1.0f,-1.0f,-1.0f },
	{ 1.0f,-1.0f,-1.0f },
	{ 1.0f, 1.0f,-1.0f },
	{ 1.0f,-1.0f,-1.0f },
	{ -1.0f,-1.0f,-1.0f },
	{ -1.0f,-1.0f,-1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f },
	{ 1.0f,-1.0f, 1.0f },
	{ -1.0f,-1.0f, 1.0f },
	{ -1.0f,-1.0f,-1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f,-1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f,-1.0f },
	{ 1.0f, 1.0f,-1.0f },
	{ 1.0f,-1.0f,-1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f },
	{ -1.0f, 1.0f,-1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f }
};

int main()
{

	int w = 120, h = 120;

	auto img = new Image(w, h);
	auto renderer = new WinConsoleRenderer(w, h);

	Mat4f projectionM = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 1 }
	};

	Mat4f scaleM = {
		{ 30, 0, 0, 0 },
		{ 0, 30, 0, 0 },
		{ 0, 0, 30, 0 },
		{ 0, 0, 0, 1 }
	};

	Mat4f rotationYM = {
		{ cos(0.1), 0, -sin(0.1), 0 },
		{ 0, 1, 0, 0 },
		{ sin(0.1), 0, cos(0.1), 0 },
		{ 0, 0, 0, 1 }
	};

	Mat4f rotationZM = {
		{ cos(0.1), -sin(0.1), 0, 0 },
		{ sin(0.1), cos(0.1), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	Mat4f rotationXM = {
		{ 1, 0, 0, 0 },
		{ 0, 0.6, -0.8, 0 },
		{ 0, 0.8, 0.6 , 0 },		
		{ 0, 0, 0, 1 }
	};

	Mat4f translationM = {
		{ 1, 0, 0, 0},
		{ 0, 1, 0, 0},
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	
	
	Vec3f cube_model[36];
	Vec3f light_dir = {0, 0, -1};

	for (unsigned int i = 0; i < 36; ++i) {
			cube_model[i] = 
				ApplyMatrix(translationM,
					ApplyMatrix(rotationYM,
						ApplyMatrix(scaleM,
							CubeTris[i])));
	}

	float totaltime = 0;
	float deltatime = 0;
	while (true) {
		auto t = clock();
		img->Clear();
		for (unsigned int i = 0; i < 12; ++i) {
			for (int j = 0; j < 3; ++j) {
				cube_model[3 * i + j] = 
					ApplyMatrix(rotationZM,
						ApplyMatrix(rotationYM,
								cube_model[3 * i + j]));
			}
			auto normal = normalized(cross(cube_model[3 * i + 2] - cube_model[3 * i], cube_model[3 * i + 1] - cube_model[3 * i]));
			auto intensity = dot(normal, light_dir);
			if (intensity > 0) {
				Color color = { 0xffffff };
				color.r *=  intensity;
				color.g *= intensity;
				color.b *= intensity;
				ImageHelper::DrawTriangle(img,
				color,
				{
					cube_model[3 * i + 0] + Vec3f{ 60, 60, 0 },
					cube_model[3 * i + 1] + Vec3f{ 60, 60, 0 },
					cube_model[3 * i + 2] + Vec3f{ 60, 60, 0 }
				});
			}

		}
		renderer->Render(img);
		renderer->ShowFps(img);
		deltatime = 1 / renderer->fps();
		totaltime += deltatime;
	}

	renderer->Render(img);

	getchar();
	return 0;
}
