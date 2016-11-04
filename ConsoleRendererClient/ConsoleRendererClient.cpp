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

const Vec3f CubeModel[36] = {
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

Matrix matrixlookat(const Vec3f &eye, const Vec3f &center, const Vec3f &up) {
	Vec3f z = normalized(eye - center);
	Vec3f x = normalized(cross(up, z));
	Vec3f y = normalized(cross(z, x));
	Matrix Minv = Mat4f{
		{ x.x, y.x, z.x, 0 },
		{ x.y, y.y, z.y, 0 },
		{ x.z, y.z, z.z, 0 },
		{ 0, 0, 0, 1 },
	};
	/*Matrix Minv = Mat4f{
		{ x.x, x.y, x.z, 0 },
		{ y.x, y.y, y.z, 0 },
		{ z.x, z.y, z.z, 0 },
		{ 0, 0, 0, 1 },
	};*/
	//Matrix Minv = Matrix::identity();
	Matrix Tr = Mat4f{
		{1, 0, 0, -center.x },
		{0, 1, 0, -center.y },
		{ 0, 0, 1, -center.z },
		{ 0, 0, 0, 1 },
	};

	return Tr * Minv;
}

int main()
{

	int w = 120, h = 120;

	auto img = new Image(w, h);
	auto renderer = new WinConsoleRenderer(w, h);
	
	Vec3f cube_mesh[36];
	Vec3f light_dir = {0, 0, -1};

	for (unsigned int i = 0; i < 36; ++i) {
	}
	auto radius = 200.0f;
	float totaltime = 0;
	float deltatime = 0;
	while (true) {
		auto View = matrixlookat({ 0.0f, 1.0f, 1000.0f }, { radius*sin(totaltime), 1.0f, 10.0f + radius*cos(totaltime) }, { 0.0f, 1.0f, 0.0f });
		auto t = clock();
		img->Clear();
		for (unsigned int i = 0; i < 12; ++i) {
			for (int j = 0; j < 3; ++j) {
				// Model matrix
				cube_mesh[3 * i + j] = CubeModel[3 * i + j];
				cube_mesh[3 * i + j] = Scale(cube_mesh[3 * i + j], { 10.0f, 10.0f, 10.0f });
				cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f, 0.6f, 0.0f });
				//cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f*totaltime, 0.6f*totaltime, 0.0f });
				//cube_mesh[3 * i + j] = Translate(cube_mesh[3 * i + j], { -50.f, -50.f, -800.0f*(0.5f + 0.5f*cos(totaltime + 3.1415f)) });
			}
			auto normal = normalized(cross(cube_mesh[3 * i + 2] - cube_mesh[3 * i], cube_mesh[3 * i + 1] - cube_mesh[3 * i]));
			// View matrix			
			cube_mesh[3 * i + 0] = ApplyMatrix(cube_mesh[3 * i + 0], View);
			cube_mesh[3 * i + 1] = ApplyMatrix(cube_mesh[3 * i + 1], View);
			cube_mesh[3 * i + 2] = ApplyMatrix(cube_mesh[3 * i + 2], View);

			// Perspective matrix
			cube_mesh[3 * i + 0] = Perspective(cube_mesh[3 * i + 0], { 0.0f, 0.0f, 200.0f });
			cube_mesh[3 * i + 1] = Perspective(cube_mesh[3 * i + 1], { 0.0f, 0.0f, 200.0f });
			cube_mesh[3 * i + 2] = Perspective(cube_mesh[3 * i + 2], { 0.0f, 0.0f, 200.0f });	
					
			auto intensity = dot(normal, light_dir);
			if (intensity < 0) intensity = 0;
				Color color = { 0x0000ff };
				color.r *=  intensity;
				color.g *= intensity;
				color.b *= intensity;
				ImageHelper::DrawTriangle(img,
				color,
				{
					cube_mesh[3 * i + 0] + Vec3f{ 60, 60, 0 },
					cube_mesh[3 * i + 1] + Vec3f{ 60, 60, 0 },
					cube_mesh[3 * i + 2] + Vec3f{ 60, 60, 0 }
				});

		}
		for (unsigned int i = 0; i < 12; ++i) {
			for (int j = 0; j < 3; ++j) {
				cube_mesh[3 * i + j] = CubeModel[3 * i + j];
				cube_mesh[3 * i + j] = Scale(cube_mesh[3 * i + j], { 20.0f, 20.0f, 20.0f });
				cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { 1.0f, 0.6f, 0.0f });
				//cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f*totaltime, 0.6f*totaltime, 0.0f });
				//cube_mesh[3 * i + j] = Translate(cube_mesh[3 * i + j], { 20.f, 20.f, -800.f*(0.5f + 0.5f*cos(totaltime)) });
			}
			auto normal = normalized(cross(cube_mesh[3 * i + 2] - cube_mesh[3 * i], cube_mesh[3 * i + 1] - cube_mesh[3 * i]));
			cube_mesh[3 * i + 0] = ApplyMatrix(cube_mesh[3 * i + 0], View);
			cube_mesh[3 * i + 1] = ApplyMatrix(cube_mesh[3 * i + 1], View);
			cube_mesh[3 * i + 2] = ApplyMatrix(cube_mesh[3 * i + 2], View);
			
			cube_mesh[3 * i + 0] = Perspective(cube_mesh[3 * i + 0], { 0.0f, 0.0f, 200.0f });
			cube_mesh[3 * i + 1] = Perspective(cube_mesh[3 * i + 1], { 0.0f, 0.0f, 200.0f });
			cube_mesh[3 * i + 2] = Perspective(cube_mesh[3 * i + 2], { 0.0f, 0.0f, 200.0f });

			auto intensity = dot(normal, light_dir);
			if (intensity < 0) intensity = 0;
			Color color = { 0xff0000 };
			color.r *= intensity;
			color.g *= intensity;
			color.b *= intensity;
			ImageHelper::DrawTriangle(img,
				color,
				{
					cube_mesh[3 * i + 0] + Vec3f{ 60, 60, 0 },
					cube_mesh[3 * i + 1] + Vec3f{ 60, 60, 0 },
				cube_mesh[3 * i + 2] + Vec3f{ 60, 60, 0 }
				});

		}
		for (unsigned int i = 0; i < 12; ++i) {
			for (int j = 0; j < 3; ++j) {
				cube_mesh[3 * i + j] = CubeModel[3 * i + j];
				cube_mesh[3 * i + j] = Scale(cube_mesh[3 * i + j], { 15.0f, 15.0f, 15.0f });
				cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -0.6f, 0.0f, 1.0f });
				//cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f*totaltime, 0.6f*totaltime, 0.0f });
				//cube_mesh[3 * i + j] = Translate(cube_mesh[3 * i + j], { -10.f, -10.f, -800.0f*(0.5f + 0.5f*sin(totaltime + 3.1415f)) });
			}
			auto normal = normalized(cross(cube_mesh[3 * i + 2] - cube_mesh[3 * i], cube_mesh[3 * i + 1] - cube_mesh[3 * i]));
			cube_mesh[3 * i + 0] = ApplyMatrix(cube_mesh[3 * i + 0], View);
			cube_mesh[3 * i + 1] = ApplyMatrix(cube_mesh[3 * i + 1], View);
			cube_mesh[3 * i + 2] = ApplyMatrix(cube_mesh[3 * i + 2], View);
			
			
			cube_mesh[3 * i + 0] = Perspective(cube_mesh[3 * i + 0], { 0.0f, 0.0f, 200.0f });
			cube_mesh[3 * i + 1] = Perspective(cube_mesh[3 * i + 1], { 0.0f, 0.0f, 200.0f });
			cube_mesh[3 * i + 2] = Perspective(cube_mesh[3 * i + 2], { 0.0f, 0.0f, 200.0f });

			auto intensity = dot(normal, light_dir);
			if (intensity < 0) intensity = 0;
			Color color = { 0x00ff00 };
			color.r *= intensity;
			color.g *= intensity;
			color.b *= intensity;
			ImageHelper::DrawTriangle(img,
				color,
				{
					cube_mesh[3 * i + 0] + Vec3f{ 60, 60, 0 },
					cube_mesh[3 * i + 1] + Vec3f{ 60, 60, 0 },
				cube_mesh[3 * i + 2] + Vec3f{ 60, 60, 0 }
				});

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
