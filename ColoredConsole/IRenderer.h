#pragma once
#include "Image.h"

namespace ColoredConsole {
	class IRenderer {
	public:
		virtual ~IRenderer() {}
		virtual void Render(const Image * img) = 0;
	};
}