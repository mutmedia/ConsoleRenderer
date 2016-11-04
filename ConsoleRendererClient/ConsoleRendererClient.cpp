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
		{ x.x, x.y, x.z, 0 },
		{ y.x, y.y, y.z, 0 },
		{ z.x, z.y, z.z, 0 },
		{ 0, 0, 0, 1.f },
	};
	Matrix Tr = Mat4f{
		{1.f, 0, 0, -center.x },
		{0, 1.f, 0, -center.y },
		{ 0, 0, 1.f, -center.z },
		{ 0, 0, 0, 1.f },
	};

	return Minv * Tr;
}

bool keys[1024];
void handleInputs(HANDLE rHnd) {
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	GetNumberOfConsoleInputEvents(rHnd, &numEvents);
	if (numEvents != 0) {
		INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];
		ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
		for (DWORD i = 0; i < numEventsRead; ++i) {
			if (eventBuffer[i].EventType == KEY_EVENT) {
				keys[eventBuffer[i].Event.KeyEvent.wVirtualKeyCode] = eventBuffer[i].Event.KeyEvent.bKeyDown;
			}
		}
	}
}

int main()
{

	int w = 120, h = 120;

	auto img = new Image(w, h);
	auto renderer = new WinConsoleRenderer(w, h);
	
	Vec3f cube_mesh[36];
	Vec3f light_dir = {0, 0, -1};

	// How many events have happened?
	HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);
	bool appIsRunning = true;

	auto radius = 200.0f;
	float totaltime = 0;
	float deltatime = 0;
	Vec3f camera_pos = { 0.0f, 0.0f, 0.0f };
	// NOTE: This might be upside down
	Vec3f camera_up = { 0.0f, 1.0f, 0.0f };
	Vec3f camera_front = { 0.0f, 0.0f, -1.0f };
	float camera_speed = 0.15f;
	float eyerotation = -3.1415/2;
	
	// TODO: move to own stuff
	
	while (appIsRunning) {
		
		handleInputs(rHnd);
		// Input stuff
		if (keys[VK_ESCAPE]) {
			appIsRunning = false;
		}
		if (keys['W']) {
			// TODO: implement += operator
			//camera_pos += camera_front * camera_speed;
			camera_pos = camera_pos + camera_front * camera_speed;
		}
		if (keys['S']) {
			camera_pos = camera_pos - camera_front * camera_speed;
		}
		if (keys['A']) {
			camera_pos = camera_pos + normalized(cross(camera_front, camera_up))*camera_speed;
		}
		if (keys['D']) {
			camera_pos = camera_pos - normalized(cross(camera_front, camera_up))*camera_speed;
		}
		if (keys[VK_LEFT]) {
			eyerotation += 0.01;
		}
		if (keys[VK_RIGHT]) {
			eyerotation -= 0.01;
		}

		camera_front = normalized(Vec3f{ cos(eyerotation), 0.0f, sin(eyerotation) });
		auto View = matrixlookat(camera_pos,
			camera_pos + camera_front,
			camera_up);
		auto t = clock();
		img->Clear();
		//for (unsigned int i = 0; i < 12; ++i) {
		//	for (int j = 0; j < 3; ++j) {
		//		// Model matrix
		//		cube_mesh[3 * i + j] = CubeModel[3 * i + j];
		//		cube_mesh[3 * i + j] = Scale(cube_mesh[3 * i + j], { 10.0f, 10.0f, 10.0f });
		//		cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f, 0.6f, 0.0f });
		//		//cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f*totaltime, 0.6f*totaltime, 0.0f });
		//		cube_mesh[3 * i + j] = Translate(cube_mesh[3 * i + j], { 0.f, 0.f, 200.f });
		//	}
		//	auto normal = normalized(cross(cube_mesh[3 * i + 2] - cube_mesh[3 * i], cube_mesh[3 * i + 1] - cube_mesh[3 * i]));
		//	// View matrix			
		//	cube_mesh[3 * i + 0] = ApplyMatrix(cube_mesh[3 * i + 0], View);
		//	cube_mesh[3 * i + 1] = ApplyMatrix(cube_mesh[3 * i + 1], View);
		//	cube_mesh[3 * i + 2] = ApplyMatrix(cube_mesh[3 * i + 2], View);

		//	// Perspective matrix
		//	cube_mesh[3 * i + 0] = Perspective(cube_mesh[3 * i + 0], { 0.0f, 0.0f, 1.0f });
		//	cube_mesh[3 * i + 1] = Perspective(cube_mesh[3 * i + 1], { 0.0f, 0.0f, 1.0f });
		//	cube_mesh[3 * i + 2] = Perspective(cube_mesh[3 * i + 2], { 0.0f, 0.0f, 1.0f });	
		//			
		//	auto intensity = dot(normal, light_dir);
		//	if (intensity < 0) intensity = 0;
		//		Color color = { 0x0000ff };
		//		color.r *=  intensity;
		//		color.g *= intensity;
		//		color.b *= intensity;
		//		ImageHelper::DrawTriangle(img,
		//		color,
		//		{
		//			cube_mesh[3 * i + 0] + Vec3f{ 60, 60, 0 },
		//			cube_mesh[3 * i + 1] + Vec3f{ 60, 60, 0 },
		//			cube_mesh[3 * i + 2] + Vec3f{ 60, 60, 0 }
		//		});

		//}
		for (unsigned int i = 0; i < 12; ++i) {
			for (int j = 0; j < 3; ++j) {
				// Model matrix
				cube_mesh[3 * i + j] = CubeModel[3 * i + j];
				cube_mesh[3 * i + j] = Scale(cube_mesh[3 * i + j], { 1.f, 1.f, 1.f });
				cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { 1.0f, 0.6f, 0.0f });
				cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f*totaltime, 0.6f*totaltime, 0.0f });
				cube_mesh[3 * i + j] = Translate(cube_mesh[3 * i + j], { 0.f, 0.f, -5.f });
			}
			auto normal = normalized(cross(cube_mesh[3 * i + 2] - cube_mesh[3 * i], cube_mesh[3 * i + 1] - cube_mesh[3 * i]));
			// View matrix			
			cube_mesh[3 * i + 0] = ApplyMatrix(cube_mesh[3 * i + 0], View);
			cube_mesh[3 * i + 1] = ApplyMatrix(cube_mesh[3 * i + 1], View);
			cube_mesh[3 * i + 2] = ApplyMatrix(cube_mesh[3 * i + 2], View);

			// Perspective matrix
			cube_mesh[3 * i + 0] = Perspective(cube_mesh[3 * i + 0], -0.1, 0.1, -0.1, 0.1, 0.1, 20.0);
			cube_mesh[3 * i + 1] = Perspective(cube_mesh[3 * i + 1], -0.1, 0.1, -0.1, 0.1, 0.1, 20.0);
			cube_mesh[3 * i + 2] = Perspective(cube_mesh[3 * i + 2], -0.1, 0.1, -0.1, 0.1, 0.1, 20.0);

			// Viewport matrix
			cube_mesh[3 * i + 0] = Viewport(cube_mesh[3 * i + 0], 0, 0, w, h);
			cube_mesh[3 * i + 1] = Viewport(cube_mesh[3 * i + 1], 0, 0, w, h);
			cube_mesh[3 * i + 2] = Viewport(cube_mesh[3 * i + 2], 0, 0, w, h);

			auto intensity = dot(normal, light_dir);
			// Ambient light
			intensity += 0.01;
			if (intensity < 0) intensity = 0;
			if (intensity > 1) intensity = 1;
			Color color = { 0x00ff00 };
			color.r *= intensity;
			color.g *= intensity;
			color.b *= intensity;
			ImageHelper::DrawTriangle(img,
				color,
				{
					cube_mesh[3 * i + 0],
					cube_mesh[3 * i + 1],
					cube_mesh[3 * i + 2]
				});

		}
		// Cool cubes moving
		//for (unsigned int i = 0; i < 12; ++i) {
		//	for (int j = 0; j < 3; ++j) {
		//		cube_mesh[3 * i + j] = CubeModel[3 * i + j];
		//		cube_mesh[3 * i + j] = Scale(cube_mesh[3 * i + j], { 20.0f, 20.0f, 20.0f });
		//		cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { 1.0f, 0.6f, 0.0f });
		//		//cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f*totaltime, 0.6f*totaltime, 0.0f });
		//		//cube_mesh[3 * i + j] = Translate(cube_mesh[3 * i + j], { 20.f, 20.f, -800.f*(0.5f + 0.5f*cos(totaltime)) });
		//	}
		//	auto normal = normalized(cross(cube_mesh[3 * i + 2] - cube_mesh[3 * i], cube_mesh[3 * i + 1] - cube_mesh[3 * i]));
		//	cube_mesh[3 * i + 0] = ApplyMatrix(cube_mesh[3 * i + 0], View);
		//	cube_mesh[3 * i + 1] = ApplyMatrix(cube_mesh[3 * i + 1], View);
		//	cube_mesh[3 * i + 2] = ApplyMatrix(cube_mesh[3 * i + 2], View);
		//	
		//	cube_mesh[3 * i + 0] = Perspective(cube_mesh[3 * i + 0], { 0.0f, 0.0f, 200.0f });
		//	cube_mesh[3 * i + 1] = Perspective(cube_mesh[3 * i + 1], { 0.0f, 0.0f, 200.0f });
		//	cube_mesh[3 * i + 2] = Perspective(cube_mesh[3 * i + 2], { 0.0f, 0.0f, 200.0f });

		//	auto intensity = dot(normal, light_dir);
		//	if (intensity < 0) intensity = 0;
		//	Color color = { 0xff0000 };
		//	color.r *= intensity;
		//	color.g *= intensity;
		//	color.b *= intensity;
		//	ImageHelper::DrawTriangle(img,
		//		color,
		//		{
		//			cube_mesh[3 * i + 0] + Vec3f{ 60, 60, 0 },
		//			cube_mesh[3 * i + 1] + Vec3f{ 60, 60, 0 },
		//		cube_mesh[3 * i + 2] + Vec3f{ 60, 60, 0 }
		//		});

		//}
		//for (unsigned int i = 0; i < 12; ++i) {
		//	for (int j = 0; j < 3; ++j) {
		//		cube_mesh[3 * i + j] = CubeModel[3 * i + j];
		//		cube_mesh[3 * i + j] = Scale(cube_mesh[3 * i + j], { 15.0f, 15.0f, 15.0f });
		//		cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -0.6f, 0.0f, 1.0f });
		//		//cube_mesh[3 * i + j] = Rotate(cube_mesh[3 * i + j], { -1.0f*totaltime, 0.6f*totaltime, 0.0f });
		//		//cube_mesh[3 * i + j] = Translate(cube_mesh[3 * i + j], { -10.f, -10.f, -800.0f*(0.5f + 0.5f*sin(totaltime + 3.1415f)) });
		//	}
		//	auto normal = normalized(cross(cube_mesh[3 * i + 2] - cube_mesh[3 * i], cube_mesh[3 * i + 1] - cube_mesh[3 * i]));
		//	cube_mesh[3 * i + 0] = ApplyMatrix(cube_mesh[3 * i + 0], View);
		//	cube_mesh[3 * i + 1] = ApplyMatrix(cube_mesh[3 * i + 1], View);
		//	cube_mesh[3 * i + 2] = ApplyMatrix(cube_mesh[3 * i + 2], View);
		//	
		//	
		//	cube_mesh[3 * i + 0] = Perspective(cube_mesh[3 * i + 0], { 0.0f, 0.0f, 200.0f });
		//	cube_mesh[3 * i + 1] = Perspective(cube_mesh[3 * i + 1], { 0.0f, 0.0f, 200.0f });
		//	cube_mesh[3 * i + 2] = Perspective(cube_mesh[3 * i + 2], { 0.0f, 0.0f, 200.0f });

		//	auto intensity = dot(normal, light_dir);
		//	if (intensity < 0) intensity = 0;
		//	Color color = { 0x00ff00 };
		//	color.r *= intensity;
		//	color.g *= intensity;
		//	color.b *= intensity;
		//	ImageHelper::DrawTriangle(img,
		//		color,
		//		{
		//			cube_mesh[3 * i + 0] + Vec3f{ 60, 60, 0 },
		//			cube_mesh[3 * i + 1] + Vec3f{ 60, 60, 0 },
		//		cube_mesh[3 * i + 2] + Vec3f{ 60, 60, 0 }
		//		});

		//}
		renderer->Render(img);
		renderer->ShowFps(img);
		deltatime = 1 / renderer->fps();
		totaltime += deltatime;
	}

	delete renderer;
	return 0;
}
