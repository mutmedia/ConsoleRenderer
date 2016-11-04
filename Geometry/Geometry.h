#pragma once

#ifdef GEOMETRY_EXPORTS
#define GEOMETRY_API __declspec(dllexport) 
#else
#define GEOMETRY_API __declspec(dllimport) 
#endif
#include <vector>
#include <cassert>

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

	// Vector operations
	GEOMETRY_API float length(const Vec3f &v);
	GEOMETRY_API Vec3f normalized(const Vec3f &v);
	GEOMETRY_API Vec3f cross(const Vec3f &u, const Vec3f &v);
	GEOMETRY_API float dot(const Vec3f &u, const Vec3f &v);

	template<size_t N>
	struct MatrixCol {
		MatrixCol() { for (size_t i = N; i--; m_[i] = 0.0f); }
		float& operator[](const size_t i) { assert(i < N); return m_[i]; }
		const float& operator[](const size_t i) const { assert(i < N); return m_[i]; }
	private:
		float m_[N];
	};

	template<size_t N>
	struct MatrixN {
		MatrixN() {
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) {
					m_[i][j] = 0;
				}
			}
		}
		MatrixN(float base[N][N]) {
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) {
					m_[i][j] = base[i][j];
				}
			}
		}
		MatrixCol<N>& operator[](const size_t i) { assert(i < N); return m_[i]; }
		const MatrixCol<N>& operator[](const size_t i) const { assert(i < N); return m_[i]; };
		static MatrixN<N> identity() {
			MatrixN<N> ret;
			for (size_t i = N; i--; )
				for (size_t j = N; j--; ret[i][j] = (i == j));
			return ret;
		}
	private:
		MatrixCol<N> m_[N];

	};
	typedef MatrixN<4> Matrix;
	typedef float Mat4f[4][4];

	#define MAT4F_IDENTITY {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}

	GEOMETRY_API Vec3f ApplyMatrix(const Vec3f  &v, const Matrix m);
	GEOMETRY_API Matrix MatrixMult(const Matrix  m1, const Matrix m2);

	// Operations using matrices
	GEOMETRY_API Vec3f Translate(const Vec3f  &v, const Vec3f &t);
	GEOMETRY_API Vec3f Rotate(const Vec3f  &v, const Vec3f &r);
	GEOMETRY_API Vec3f Scale(const Vec3f  &v, const Vec3f &s);
	GEOMETRY_API Vec3f Perspective(const Vec3f  &v, const Vec3f &camera);

	struct Triangle {
		Vec3f v_0, v_1, v_2;
	};

	struct Rectangle {
		Vec2i top_left, bottom_right;
	};

	
	template<size_t N>
	MatrixN<N> operator*(const MatrixN<N>& lhs, const MatrixN<N>& rhs)
	{
		MatrixN<N> res;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				res[i][j] = 0;
				for (int k = 0; k < N; ++k) {
					res[i][j] += lhs[i][k] * rhs[k][j];
				}
				
			}
		}
		return res;
	}

}
