// Geometry.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Geometry.h"

namespace Geometry {
	Vec2i Vec2i::operator+(const Vec2i b) const {
		return{ x + b.x, y + b.y };
	}

	Vec3f cross(Vec3f u, Vec3f v) {
		return{ u.y * v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x };
	}

	Vec4f Vec4(Vec3f v3) {
		return{ v3.x, v3.y, v3.z, 1 };
	}

	/*Vec4f ApplyMatrix(Mat4f m, Vec4f v) {
		float result[4];
		for (int i = 0; i < 4; ++i) {
			result[i] = m[0][i] * v.x + m[1][i] * v.y + m[2][i] * v.z + m[3][i] * v.w;
		}
		return{ result[0], result[1], result[2], result[3] };
	}*/

	Vec3f ApplyMatrix(Mat4f m, Vec3f v) {
		float result[4];
		for (int i = 0; i < 4; ++i) {
			result[i] = m[0][i] * v.x + m[1][i] * v.y + m[2][i] * v.z + m[3][i];
		}
		return{ result[0], result[1], result[2] };
	}


}

