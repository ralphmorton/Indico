
#ifndef _INDICOVECTOR3_H_
#define _INDICOVECTOR3_H_

#include "IndicoMath.h"

#include <assert.h>

namespace Indico
{
	class Vector3
	{
	public:
		float x,y,z;

	public:
		inline Vector3() {}
        inline Vector3(const float fX, const float fY, const float fZ) : x(fX), y(fY), z(fZ) {}
        inline explicit Vector3(const float val) : x(val) , y(val) , z(val) {}

		inline float operator [](const unsigned int i) const
        {
            assert(i < 3);
            return *(&x+i);
        }

		inline float& operator [](const unsigned int i)
        {
            assert(i < 3);
            return *(&x+i);
        }

        inline Vector3& operator = (const Vector3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;

            return *this;
        }

        inline Vector3& operator = (const float scalar)
        {
            x = scalar;
            y = scalar;
            z = scalar;

            return *this;
        }

        inline bool operator == (const Vector3& other) const
        {
            return (x == other.x && y == other.y && z == other.z);
        }

        inline bool operator != (const Vector3& other) const
        {
            return (x != other.x || y != other.y || z != other.z);
        }

        inline Vector3 operator + (const Vector3& other) const
        {
            return Vector3(
                x + other.x,
                y + other.y,
                z + other.z);
        }

        inline Vector3 operator - (const Vector3& other) const
        {
            return Vector3(
                x - other.x,
                y - other.y,
                z - other.z);
        }

        inline Vector3 operator * (const float scalar) const
        {
            return Vector3(
                x * scalar,
                y * scalar,
                z * scalar);
        }

        inline Vector3 operator * (const Vector3& other) const
        {
            return Vector3(
                x * other.x,
                y * other.y,
                z * other.z);
        }

        inline Vector3 operator / (const float scalar) const
        {
            assert(scalar != 0.0);

            float fInv = 1.0f / scalar;

            return Vector3(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline Vector3 operator / ( const Vector3& other) const
        {
            return Vector3(
                x / other.x,
                y / other.y,
                z / other.z);
        }

        inline Vector3 operator - () const
        {
            return Vector3(-x, -y, -z);
        }

        inline Vector3& operator += (const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;

            return *this;
        }

        inline Vector3& operator -= (const Vector3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;

            return *this;
        }

        inline Vector3& operator *= (const float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        inline Vector3& operator /= (const float scalar)
        {
            assert(scalar != 0.0);

            float fInv = 1.0f / scalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline float length() const
        {
            return Math::sqrt(x * x + y * y + z * z);
        }

        inline float lengthSquared() const
        {
            return x * x + y * y + z * z;
        }

        inline float distance(const Vector3& other) const
        {
            return (*this - other).length();
        }

        inline float distanceSquared(const Vector3& other) const
        {
            return (*this - other).lengthSquared();
        }

        inline float dot(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        inline float normalise()
        {
            float fLength = Math::sqrt( x * x + y * y + z * z );

            float fInvLength = 1.0f / fLength;
            x *= fInvLength;
            y *= fInvLength;
            z *= fInvLength;

            return fLength;
        }

        inline Vector3 cross(const Vector3& other) const
        {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x);
        }

        inline Vector3 midPoint(const Vector3& vec) const
        {
            return Vector3(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f,
                ( z + vec.z ) * 0.5f );
        }

        inline bool operator < (const Vector3& other) const
        {
            if(x < other.x && y < other.y && z < other.z)
                return true;
            return false;
        }

        inline bool operator > (const Vector3& other) const
        {
            if( x > other.x && y > other.y && z > other.z )
                return true;
            return false;
        }

        inline Vector3 perpendicular() const
        {
            static const float fSquareZero = (float)(1e-06 * 1e-06);

            Vector3 perp = this->cross( Vector3::UNIT_X );
            if( perp.lengthSquared() < fSquareZero )
            {
                perp = this->cross(Vector3::UNIT_Y);
            }
			perp.normalise();

            return perp;
        }

		inline float angleBetween(const Vector3& dest)
		{
			float lenProduct = length() * dest.length();

			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;

			float f = dot(dest) / lenProduct;

			f = Math::clamp(f, (float)-1.0, (float)1.0);
			return Math::acos(f);
		}

        inline Vector3 normalised() const
        {
            Vector3 ret = *this;
            ret.normalise();
            return ret;
        }

        inline Vector3 reflect(const Vector3& normal) const
        {
            return Vector3(*this - (normal * (2 * this->dot(normal))));
        }

		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z);
		}

        static const Vector3 ZERO;
		static const Vector3 ONE;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
	};
}

#endif
