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

	float length(const Vec3f &v)
	{
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}
	Vec3f normalized(const Vec3f &v)
	{
		return v * InvSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}
	Vec3f cross(const Vec3f &u, const Vec3f &v) {
		return{ u.y * v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x };
	}
	float dot(const Vec3f &u, const Vec3f &v)
	{
		return u.x * v.x + u.y * v.y + u.z*v.z;
	}
	

	Vec3f ApplyMatrix(const Vec3f & v, const Matrix m) {
		float result[4];
		Vec3f ret;
		for (int i = 0; i < 4; ++i) {
			result[i] = m[i][0] * v.x + m[i][1] * v.y + m[i][2] * v.z + m[i][3];
		}
		ret = { result[0], result[1], result[2] };
		if(result[3] != 1) {
			ret = ret * (1 / result[3]);
		}
		return ret;
	}

	GEOMETRY_API Vec3f Translate(const Vec3f & v, const Vec3f & t)
	{
		return ApplyMatrix(v,
		Mat4f{
			{1, 0, 0, t.x},
			{0, 1, 0, t.y},
			{0, 0, 1, t.z},
			{0, 0, 0, 1},
		});
	}

	GEOMETRY_API Vec3f Rotate(const Vec3f & v, const Vec3f & r)
	{
		// TODO: change to quaternions?
		// Order is XYZ by default
		Vec3f rv = v;

		if(r.x != 0) {
			rv = ApplyMatrix(rv,
				Mat4f{
					{ 1, 0, 0, 0 },
					{ 0, cos(r.x), -sin(r.x), 0 },
					{ 0, sin(r.x), cos(r.x), 0 },
					{ 0, 0, 0, 1 },
			});
		}
		if (r.y != 0) {
			rv = ApplyMatrix(rv,
				Mat4f{
					{ cos(r.y), 0, -sin(r.y), 0 },
					{ 0, 1, 0, 0 },
					{ sin(r.y), 0, cos(r.y), 0 },
					{ 0, 0, 0, 1 },
			});
		}
		if (r.z != 0) {
			rv = ApplyMatrix(rv,
				Mat4f{
					{ cos(r.z), -sin(r.z), 0, 0 },
					{ sin(r.z), cos(r.z), 0, 0 },
					{ 0, 0, 1, 0 },
					{ 0, 0, 0, 1 },
			});
		}
		return rv;
	}

	GEOMETRY_API Vec3f Scale(const Vec3f & v, const Vec3f & s)
	{
		return ApplyMatrix(v,
			Mat4f{
				{ s.x, 0, 0, 0 },
				{ 0, s.y, 0, 0 },
				{ 0, 0, s.z, 0 },
				{ 0, 0, 0, 1 },
		});
	}

	GEOMETRY_API Vec3f Perspective(const Vec3f & v, float l, float r, float b, float t, float n, float f)
	{
		// TODO: understand right and left hand coordinates and why i have to make stuff negative
		return ApplyMatrix(v,
			Mat4f{
				{ -2.f*n/(r-l)	, 0			, (r+l)/(r-l)	, 0 },
				{ 0			, -2.f*n/(t-b)	, (t+b)/(t-b)	, 0 },
				{ 0			, 0			, (f+n)/(f-n)	, 2.f*f*n/(f-n) },
				{ 0			, 0			, -1		, 0 },
		});
	}

	GEOMETRY_API Vec3f Viewport(const Vec3f & v, float x, float y, float w, float h)
	{
		float depth = 255.0f;
		return ApplyMatrix(v, Mat4f{
			{w/2.f, 0, 0, x + w/2.f},
			{0, h/2.f, 0, y + h/2.f},
			{0, 0, 1.f, 1.f},
			{0, 0, 0, 1.f}
		});
	}

	std::vector<Vec3f> ApplyMatrix(const Matrix m, const std::vector<Vec3f> vs)
	{
		auto list = std::vector<Vec3f>(vs);
		for (auto v : vs) {
			list.push_back(ApplyMatrix(v, m));
		}
		return list;
	}


}
