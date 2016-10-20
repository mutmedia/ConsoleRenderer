// ConsoleRendererClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinConsoleRenderer.h"
#include "Image.h"
#include "ImageHelper.h"
#include "Geometry.h"
#include <vector>



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
		{ 0, 1, 0,  0},
		{ 0, 0, 1, -0.5 },
		{ 0, 0, 0, 1 }
	};

	
	
	Vec3f cubemodel[36];
	Color colors[12] = {
		0x0000ff,
		0x00ff00,
		0x00ffff,
		0xff0000,
		0xff00ff,
		0xffffff,
		0x0000ff,
		0x00ff00,
		0x00ffff,
		0xff0000,
		0xff00ff,
		0xffffff
	};

	for (unsigned int i = 0; i < 12; ++i) {
		for (int j = 0; j < 3; ++j) {
			cubemodel[3 * i + j] = 
				ApplyMatrix(translationM,
					//ApplyMatrix(rotationZM,
						ApplyMatrix(rotationYM,
							//ApplyMatrix(rotationXM,
							ApplyMatrix(scaleM,
								CubeTris[3 * i + j])));//);
		}
		ImageHelper::DrawTriangle(img, 
			{colors[(i) % 12]},
			{
				{ int(cubemodel[3*i+0].x + 60), int(cubemodel[3*i+0].y + 60) },
				{ int(cubemodel[3*i+1].x + 60), int(cubemodel[3*i+1].y + 60) },
				{ int(cubemodel[3*i+2].x + 60), int(cubemodel[3*i+2].y + 60) },					
			});

	}

	while (true) {
		img->Clear();
		for (unsigned int i = 0; i < 12; ++i) {
			for (int j = 0; j < 3; ++j) {
				cubemodel[3 * i + j] = 
					ApplyMatrix(rotationZM,
						ApplyMatrix(rotationYM,
								cubemodel[3 * i + j]));
			}
			ImageHelper::DrawTriangle(img,
			{ colors[(i) % 12] },
			{
				{ int(cubemodel[3 * i + 0].x + 60), int(cubemodel[3 * i + 0].y + 60) },
				{ int(cubemodel[3 * i + 1].x + 60), int(cubemodel[3 * i + 1].y + 60) },
				{ int(cubemodel[3 * i + 2].x + 60), int(cubemodel[3 * i + 2].y + 60) },
			});

		}
		renderer->Render(img);
		renderer->ShowFps(img);
	}

	renderer->Render(img);

	getchar();
	return 0;
}
