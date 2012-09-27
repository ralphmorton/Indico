
#ifndef _INDICOVECTOR4_H_
#define _INDICOVECTOR4_H_

#include "IndicoMath.h"
#include "IndicoVector3.h"

#include <assert.h>

namespace Indico
{
	class Vector4
	{
	public:
		float x,y,z,w;

	public:
		inline Vector4() { }
        inline Vector4(const float fX, const float fY, const float fZ, const float fW) : x(fX), y(fY), z(fZ), w(fW) {}
        inline explicit Vector4(const float val) : x(val), y(val), z(val), w(val) {}
        inline explicit Vector4(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(1.0f) {}

		inline float operator [](const unsigned int i) const
        {
            assert( i < 4 );
            return *(&x+i);
        }

		inline float& operator [](const unsigned int i)
        {
            assert( i < 4 );
            return *(&x+i);
        }

        inline Vector4& operator = (const Vector4& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;

            return *this;
        }

		inline Vector4& operator = (const float scalar)
		{
			x = scalar;
			y = scalar;
			z = scalar;
			w = scalar;
			return *this;
		}

        inline bool operator == (const Vector4& other) const
        {
            return (x == other.x &&
                y == other.y &&
                z == other.z &&
                w == other.w);
        }

        inline bool operator != (const Vector4& other) const
        {
            return (x != other.x ||
                y != other.y ||
                z != other.z ||
                w != other.w);
        }

        inline Vector4& operator = (const Vector3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = 1.0f;
            return *this;
        }

        inline Vector4 operator + (const Vector4& other) const
        {
            return Vector4(
                x + other.x,
                y + other.y,
                z + other.z,
                w + other.w);
        }

        inline Vector4 operator - ( const Vector4& rkVector ) const
        {
            return Vector4(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z,
                w - rkVector.w);
        }

        inline Vector4 operator * (const float scalar) const
        {
            return Vector4(
                x * scalar,
                y * scalar,
                z * scalar,
                w * scalar);
        }

        inline Vector4 operator * (const Vector4& rhs) const
        {
            return Vector4(
                rhs.x * x,
                rhs.y * y,
                rhs.z * z,
                rhs.w * w);
        }

        inline Vector4 operator / (const float scalar) const
        {
            assert(scalar != 0.0);

            float fInv = 1.0f / scalar;

            return Vector4(
                x * fInv,
                y * fInv,
                z * fInv,
                w * fInv);
        }

        inline Vector4 operator - () const
        {
            return Vector4(-x, -y, -z, -w);
        }

        inline Vector4& operator += (const Vector4& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;

            return *this;
        }

        inline Vector4& operator -= (const Vector4& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;

            return *this;
        }

        inline Vector4& operator *= (const float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        inline Vector4& operator /= (const float scalar)
        {
            assert(scalar != 0.0);

            float fInv = 1.0f / scalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;
            w *= fInv;

            return *this;
        }

        inline float dot(const Vector4& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
        }

		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}

        static const Vector4 ZERO;
	};
}

#endif
