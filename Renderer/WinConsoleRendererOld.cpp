#include "WinConsoleRenderer.h"
#include <stdio.h>
#include "ConsoleColorList.h"
#include "ConsoleColorMap.h"



namespace WinConsoleRenderer {
	WinConsoleRenderer::WinConsoleRenderer(int sizeX, int sizeY) : _sizeX(sizeX), _sizeY(sizeY)
	{
		_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(_hConsole, &csbiInfo);
		_baseAtr = csbiInfo.wAttributes;
	}

	WinConsoleRenderer::~WinConsoleRenderer()
	{
	}

	void WinConsoleRenderer::render(const Image * img)
	{
		// Get correct data for sub and super sampling of img
		int b_red, b_green, b_blue, b_intensity, red, green, blue, intensity, mixLevel;


		SetConsoleCursorPosition(_hConsole, { 0, 0 });
		for (auto y = 0; y < _sizeY; y++)
		{
			printf("\n\t");
			for (auto x = 0; x < _sizeX; x++)
			{
				Color c = img->getColor(x, y);
				int chosenColor = 0;
				int dif = 9999999999;
				int key = 0;
				for (auto i = 0; i < COLOR_LIST_SIZE; i++)
				{
					auto testColor = COLOR_LIST[i];
					auto difR = abs(c.r - ((testColor & 0xff0000) >> (16)));
					auto difG = abs(c.g - ((testColor & 0x00ff00) >> (8)));
					auto difB = abs(c.b - (testColor & 0x0000ff));

					auto newDif = difR + difG + difB;
					if (newDif < dif)
					{
						dif = newDif;
						key = testColor;
					}
				}
				int mappedValue = 0;
				mappedValue = CONSOLE_COLOR_MAP.at(key);

				mixLevel = (0b11 & mappedValue);
				intensity = (0b1 & (mappedValue >> 5));
				blue = (0b1 & (mappedValue >> 4));
				green = (0b1 & (mappedValue >> 3));
				red = (0b1 & (mappedValue >> 2));
				b_intensity = (0b1 & (mappedValue >> 9));
				b_blue = (0b1 & (mappedValue >> 8));
				b_green = (0b1 & (mappedValue >> 7));
				b_red = (0b1 & (mappedValue >> 6));


				WORD cAttributes = 0;

				// Transd			

				cAttributes |= red ? FOREGROUND_RED : 0;
				cAttributes |= green ? FOREGROUND_GREEN : 0;
				cAttributes |= blue ? FOREGROUND_BLUE : 0;
				cAttributes |= intensity ? FOREGROUND_INTENSITY : 0;
				cAttributes |= b_red ? BACKGROUND_RED : 0;
				cAttributes |= b_green ? BACKGROUND_GREEN : 0;
				cAttributes |= b_blue ? BACKGROUND_BLUE : 0;
				cAttributes |= b_intensity ? BACKGROUND_INTENSITY : 0;

				SetConsoleTextAttribute(_hConsole, cAttributes);
				int foreShading[] = { 0x00, 0xb0, 0xb1, 0xb2, 0xdb };
				printf("%c", foreShading[mixLevel + 1]);
				//remove
			}
			SetConsoleTextAttribute(_hConsole, _baseAtr);
		}
		SetConsoleTextAttribute(_hConsole, _baseAtr);
	}

	void WinConsoleRenderer::TestColor()
	{
		// Get correct data for sub and super sampling of img
		int b_red, b_green, b_blue, b_intensity, red, green, blue, intensity, mixLevel;
		int count = 0;

		SetConsoleCursorPosition(_hConsole, { 0, 0 });
		printf("COLOR TEST-------------------------\n\n");
		for (auto y = 0; y < _sizeY; y++)
		{
			printf("\n\t");
			for (auto x = 0; x < _sizeX; x++)
			{
				mixLevel = (0b11 & count);
				intensity = (0b1 & (count >> 5));
				blue = (0b1 & (count >> 4));
				green = (0b1 & (count >> 3));
				red = (0b1 & (count >> 2));
				b_intensity = (0b1 & (count >> 9));
				b_blue = (0b1 & (count >> 8));
				b_green = (0b1 & (count >> 7));
				b_red = (0b1 & (count >> 6));
				//printf("%d r:%d g:%d b:%d i:%d mix:%d ", count, red, green, blue, intensity, mixLevel);
				//printf("b_r:%d b_g:%d b_b:%d b_i:%d ", b_red, b_green, b_blue, b_intensity);
				count++;
				//Color c = img->getColor(x, y);

				WORD cAttributes = 0;

				// Transd			

				cAttributes |= red ? FOREGROUND_RED : 0;
				cAttributes |= green ? FOREGROUND_GREEN : 0;
				cAttributes |= blue ? FOREGROUND_BLUE : 0;
				cAttributes |= intensity ? FOREGROUND_INTENSITY : 0;
				cAttributes |= b_red ? BACKGROUND_RED : 0;
				cAttributes |= b_green ? BACKGROUND_GREEN : 0;
				cAttributes |= b_blue ? BACKGROUND_BLUE : 0;
				cAttributes |= b_intensity ? BACKGROUND_INTENSITY : 0;

				SetConsoleTextAttribute(_hConsole, cAttributes);
				int foreShading[] = { 0x00, 0xb0, 0xb1, 0xb2, 0xdb };
				printf("%c", foreShading[mixLevel + 1]);
				//printf("%c", foreShading[mixLevel + 1]);
				//printf("%c", foreShading[mixLevel + 1]);
				//printf("%c", foreShading[mixLevel + 1]);
				//remove
			}
			SetConsoleTextAttribute(_hConsole, _baseAtr);
		}
		SetConsoleTextAttribute(_hConsole, _baseAtr);
		printf("\n\n\nEND OF COLOR TEST-------------------------");
	}
}
