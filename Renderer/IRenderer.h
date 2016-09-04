#pragma once
#include "Image.h"

namespace WinConsoleRenderer {
	class IRenderer {
	public:
		virtual ~IRenderer() {}
		virtual void render(const Image * img) = 0;
	};
}