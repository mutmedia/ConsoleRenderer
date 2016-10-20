#pragma once

#ifdef GEOMETRY_EXPORTS
#define GEOMETRY_API __declspec(dllexport) 
#else
#define GEOMETRY_API __declspec(dllimport) 
#endif

namespace Geometry {
	struct Vec2i {
		int x, y;
		Vec2i operator+(const Vec2i b) const;
	};

	struct Vec3f {
		float x, y, z;
	};

	struct Vec4f {
		float x, y, z, w;
	};
	
	GEOMETRY_API Vec4f Vec4(Vec3f v3);

	typedef float Mat4f[4][4];

	struct Triangle {
		Vec2i v_0, v_1, v_2;
	};

	//GEOMETRY_API Vec4f ApplyMatrix(Mat4f m, Vec4f v);
	GEOMETRY_API Vec3f ApplyMatrix(Mat4f m, Vec3f v);

	struct Rectangle {
		Vec2i top_left, bottom_right;
	};

	GEOMETRY_API Vec3f cross(Vec3f u, Vec3f v);
}