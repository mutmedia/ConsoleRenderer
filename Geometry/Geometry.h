#pragma once

#ifdef GEOMETRY_EXPORTS
#define GEOMETRY_API __declspec(dllexport) 
#else
#define GEOMETRY_API __declspec(dllimport) 
#endif
#include <vector>

namespace Geometry {
	struct Vec2i {
		int x, y;
		GEOMETRY_API Vec2i operator+(const Vec2i b) const;
		GEOMETRY_API Vec2i operator-(const Vec2i b) const;
	};

	struct Vec3f {
		float x, y, z;
		GEOMETRY_API Vec3f operator+(const Vec3f b) const;
		GEOMETRY_API Vec3f operator-(const Vec3f b) const;
		GEOMETRY_API Vec3f operator*(const float k) const;
	};

	GEOMETRY_API float length(Vec3f v);
	GEOMETRY_API Vec3f normalized(Vec3f v);
	GEOMETRY_API Vec3f cross(Vec3f u, Vec3f v);
	GEOMETRY_API float dot(Vec3f u, Vec3f v);

	

	typedef float Mat4f[4][4];

	struct Triangle {
		Vec3f v_0, v_1, v_2;
	};

	GEOMETRY_API Vec3f ApplyMatrix(const Mat4f m, const Vec3f  &v);

	struct Rectangle {
		Vec2i top_left, bottom_right;
	};

	
}
