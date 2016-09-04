// ConsoleRendererClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinConsoleRenderer.h"
#include "Image.h"

void MakeX(ColoredConsole::Image * img, int posX, int posY, int size, ColoredConsole::Color c)
{
	for (auto i = 0; i < size; i++)
	{
		img->SetColor(posX + i, posY + i, c);
		img->SetColor(posX + i, posY + size - i - 1, c);
	}
}

// Helper on ConsoleColors::Image?
void Circle(ColoredConsole::Image * img, int centerX, int centerY, int radius, ColoredConsole::Color c)
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

void MakeGrid(ColoredConsole::Image * img, int size, ColoredConsole::Color c)
{
	for (auto i = 0; i < size; i++)
	{
		img->SetColor(size / 3, i, c);
		img->SetColor(2 * size / 3, i, c);
		img->SetColor(i, size / 3, c);
		img->SetColor(i, 2 * size / 3, c);
	}
}

void PutX(ColoredConsole::Image * img, int x, int y, int gridsize)
{
	MakeX(img, x * gridsize / 3 + 1, y * gridsize / 3 + 1, gridsize / 3 -1, { 0xff0000 });
}

void PutO(ColoredConsole::Image * img, int x, int y, int gridsize)
{
	Circle(img, gridsize / 6 + x * gridsize / 3, gridsize / 6 + y * gridsize / 3, gridsize / 6 - 1, { 0x0000ff });
}

int main()
{

	int w = 100, h = 100;

	auto img = new ColoredConsole::Image(w, h);
	auto renderer = new ColoredConsole::WinConsoleRenderer(w, h);
	// Draw X
	MakeGrid(img, 30, { 0xffffff });
	PutO(img, 0, 0, 30);
	PutX(img, 1, 1, 30);
	PutO(img, 1, 0, 30);

	
	//FPS Test
	while (true) {
		renderer->Render(img);
		renderer->ShowFps(img);
	}	

	getchar();
	return 0;
}



