// ColoredConsole.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "WinConsoleRenderer.h"
#include <stdio.h>
#include "ConsoleColorList.h"
#include "ConsoleColorMap.h"
#include <time.h>

namespace ColoredConsole {
WinConsoleRenderer::WinConsoleRenderer(int sizeX, int sizeY) : size_x_(sizeX), size_y_(sizeY) {
	console_handle_ = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(console_handle_, &csbiInfo);
	base_atr_ = csbiInfo.wAttributes;

	char_info_array_ = static_cast<CHAR_INFO *>(malloc(sizeof(CHAR_INFO) * size_x_ * size_y_));
	char_info_array_size_ = {size_x_ , size_y_};

	buffer_rectangle_ = { 0, 0, size_x_ - 1, size_y_ - 1 };

	fps_ = 0;
}

WinConsoleRenderer::~WinConsoleRenderer() {}

void WinConsoleRenderer::Render(const Image* img) {
	auto begin_time = clock();
	// Get correct data for sub and super sampling of img

	for (auto y = 0; y < size_y_; y++) {
		for (auto x = 0; x < size_x_; x++) {
			auto c = img->GetColor(x, y);
			auto char_info = RgbToCharInfo(c);
			char_info_array_[x + size_x_ * y] = char_info;
		}
	}
	
	WriteConsoleOutputA(console_handle_, char_info_array_, char_info_array_size_, {0}, &buffer_rectangle_);
	SetConsoleTextAttribute(console_handle_, base_atr_);

	fps_ = CLOCKS_PER_SEC / float(clock() - begin_time);
}

void WinConsoleRenderer::ShowFps(const Image* img) {
	SetConsoleCursorPosition(console_handle_, {size_x_, size_y_});
	printf("FPS: %f", fps_);
}

void WinConsoleRenderer::TestColor() {
	// Get correct data for sub and super sampling of img
	auto count = 0;

	for (auto y = 0; y < 32; y++) {
		for (auto x = 0; x < 32; x++) {			

			auto char_info = ColorCodeToCharInfo(count);
			char_info_array_[x + 32 * y] = char_info;

			count++;
		}
	}
	SetConsoleTextAttribute(console_handle_, base_atr_);
	WriteConsoleOutputA(console_handle_, char_info_array_, { 32, 32 }, { 0 }, &buffer_rectangle_);
	SetConsoleCursorPosition(console_handle_, { 0, 32 });
	printf("\n\n\nTHESE ARE ALL THE POSSIBLE COLOR CODES");
}

CHAR_INFO WinConsoleRenderer::ColorCodeToCharInfo(int color_code) const {	
	CHAR_INFO char_info = {};
	
	auto mixLevel = (0b11 & color_code);
	auto intensity = (0b1 & (color_code >> 5));
	auto blue = (0b1 & (color_code >> 4));
	auto green = (0b1 & (color_code >> 3));
	auto red = (0b1 & (color_code >> 2));
	auto b_intensity = (0b1 & (color_code >> 9));
	auto b_blue = (0b1 & (color_code >> 8));
	auto b_green = (0b1 & (color_code >> 7));
	auto b_red = (0b1 & (color_code >> 6));			
	
	char_info.Attributes |= red ? FOREGROUND_RED : 0;
	char_info.Attributes |= green ? FOREGROUND_GREEN : 0;
	char_info.Attributes |= blue ? FOREGROUND_BLUE : 0;
	char_info.Attributes |= intensity ? FOREGROUND_INTENSITY : 0;
	char_info.Attributes |= b_red ? BACKGROUND_RED : 0;
	char_info.Attributes |= b_green ? BACKGROUND_GREEN : 0;
	char_info.Attributes |= b_blue ? BACKGROUND_BLUE : 0;
	char_info.Attributes |= b_intensity ? BACKGROUND_INTENSITY : 0;

	char_info.Char.AsciiChar = kForeShading[mixLevel + 1];

	return char_info;
}

int WinConsoleRenderer::RgbToColorCode(Color color) const {
	int dif = 9999999999;
	auto key = 0;
	for (auto i = 0; i < kColorListSize; i++) {
		auto testColor = kColorList[i];
		auto difR = abs(color.r - ((testColor & 0xff0000) >> (16)));
		auto difG = abs(color.g - ((testColor & 0x00ff00) >> (8)));
		auto difB = abs(color.b - (testColor & 0x0000ff));

		auto newDif = difR + difG + difB;
		if (newDif < dif) {
			dif = newDif;
			key = testColor;
		}
	}
	return kConsoleColorMap.at(key);
}

CHAR_INFO WinConsoleRenderer::RgbToCharInfo(Color color) const {
	return ColorCodeToCharInfo(RgbToColorCode(color));
}

}
