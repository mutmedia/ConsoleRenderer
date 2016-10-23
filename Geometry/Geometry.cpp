// Geometry.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Geometry.h"

// Helper
float InvSqrt(float x) {
	float xhalf = 0.5f * x;
	int i = *(int*)&x;            // store floating-point bits in integer
	i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
	x = *(float*)&i;              // convert new bits into float
	x = x*(1.5f - xhalf*x*x);     // One round of Newton's method
	return x;
}

namespace Geometry {
	

	Vec2i Vec2i::operator+(const Vec2i b) const {
		return{ x + b.x, y + b.y };
	}
	Vec2i Vec2i::operator-(const Vec2i b) const {
		return{ x - b.x, y - b.y };
	}

	Vec3f Vec3f::operator+(const Vec3f b) const {
		return{ x + b.x, y + b.y, z + b.z };
	}
	Vec3f Vec3f::operator-(const Vec3f b) const {
		return{ x - b.x, y - b.y, z - b.z };
	}
	Vec3f Vec3f::operator*(const float k) const {
		return{ k*x, k*y, k*z };
	}
	Vec3f cross(Vec3f u, Vec3f v) {
		return{ u.y * v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x };
	}
	float dot(Vec3f u, Vec3f v)
	{
		return u.x * v.x + u.y * v.y + u.z*v.z;
	}
	float length(Vec3f v)
	{
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}
	Vec3f normalized(Vec3f v)
	{
		return v * InvSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}

	Vec3f ApplyMatrix(const Mat4f m, const Vec3f & v) {
		float result[4];
		for (int i = 0; i < 4; ++i) {
			result[i] = m[i][0] * v.x + m[i][1] * v.y + m[i][2] * v.z + m[3][i];
		}
		return{ result[0], result[1], result[2] };
	}

	std::vector<Vec3f> ApplyMatrix(const Mat4f m, const std::vector<Vec3f> vs)
	{
		auto list = std::vector<Vec3f>(vs);
		for (auto v : vs) {
			list.push_back(ApplyMatrix(m, v));
		}
		return list;
	}


}
