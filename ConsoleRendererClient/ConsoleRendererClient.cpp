// ConsoleRendererClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinConsoleRenderer.h"
#include "Image.h"
#include "ImageHelper.h"

using namespace ColoredConsole;

void MakeX(Image * img, int posX, int posY, int size, Color c)
{
	for (auto i = 0; i < size; i++)
	{
		img->SetColor(posX + i, posY + i, c);
		img->SetColor(posX + i, posY + size - i - 1, c);
	}
}

void Circle(Image * img, int centerX, int centerY, int radius, Color c)
{
	auto x = radius;
	auto y = 0;
	auto err = 0;
	
	// Bresenham Circle Drawing Algorithm
	while (x >= y)
	{
		img->SetColor(centerX + x, centerY + y, c);
		img->SetColor(centerX + y, centerY + x, c);
		img->SetColor(centerX - y, centerY + x, c);
		img->SetColor(centerX - x, centerY + y, c);
		img->SetColor(centerX - x, centerY - y, c);
		img->SetColor(centerX - y, centerY - x, c);
		img->SetColor(centerX + y, centerY - x, c);
		img->SetColor(centerX + x, centerY - y, c);

		y += 1;
		err += 1 + 2 * y;
		if (2 * (err - x) + 1 > 0)
		{
			x -= 1;
			err += 1 - 2 * x;
		}
	}
}

void MakeGrid(Image * img, int size, Color c)
{
	for (auto i = 0; i < size; i++)
	{
		img->SetColor(size / 3, i, c);
		img->SetColor(2 * size / 3, i, c);
		img->SetColor(i, size / 3, c);
		img->SetColor(i, 2 * size / 3, c);
	}
}

void PutX(Image * img, int x, int y, int gridsize)
{
	MakeX(img, x * gridsize / 3 + 1, y * gridsize / 3 + 1, gridsize / 3 -1, { 0xff0000 });
}

void PutO(Image * img, int x, int y, int gridsize)
{
	Circle(img, gridsize / 6 + x * gridsize / 3, gridsize / 6 + y * gridsize / 3, gridsize / 6 - 1, { 0x0000ff });
}

int main()
{

	int w = 100, h = 100;

	auto img = new Image(w, h);
	auto renderer = new WinConsoleRenderer(w, h);
	// Draw X
	//MakeGrid(img, 30, { 0xffffff });
	//PutO(img, 0, 0, 30);
	//PutX(img, 1, 1, 30);
	//PutO(img, 1, 0, 30);

	//ImageHelper::DrawLine(img, { 0x00ffff }, 50, 10, 100-10, 100-50);
	ImageHelper::DrawTriangle(img, { 0xff0000 }, { { 30, 20 }, { 30, 30 }, { 20, 30 } });
	ImageHelper::DrawTriangle(img, { 0x00ff00 }, { { 29, 20 },{ 20, 29 },{ 20, 20 } });
	//FPS Test
	while (true) {
		renderer->Render(img);
		renderer->ShowFps(img);
	}

	getchar();
	return 0;
}



