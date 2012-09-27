
#ifndef _INDICOVECTOR2_H_
#define _INDICOVECTOR2_H_

#include "IndicoMath.h"

#include <assert.h>

namespace Indico
{
	class Vector2
	{
	public:
		float x,y;

	public:
		inline Vector2() : x(0), y(0) {}
		inline Vector2(float fx, float fy) : x(fx), y(fy) {}

		inline float operator [](const unsigned int i) const
        {
            assert(i < 2);
            return *(&x+i);
        }

		inline float& operator [](const unsigned int i)
        {
            assert(i < 2);
            return *(&x+i);
        }

		inline Vector2& operator =(const Vector2& other)
        {
            x = other.x;
            y = other.y;

            return *this;
        }

		inline Vector2& operator =(const float scalar)
		{
			x = scalar;
			y = scalar;

			return *this;
		}

        inline bool operator == (const Vector2& other) const
        {
            return (x == other.x && y == other.y);
        }

        inline bool operator !=(const Vector2& other) const
        {
            return (x != other.x || y != other.y );
        }

        inline Vector2 operator +(const Vector2& other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        inline Vector2 operator -(const Vector2& other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        inline Vector2 operator *(const float scalar) const
        {
            return Vector2(x * scalar, y * scalar);
        }

        inline Vector2 operator *(const Vector2& other) const
        {
            return Vector2(x * other.x, y * other.y);
        }

        inline Vector2 operator /(const float scalar) const
        {
            assert(scalar != 0.0);

            float fInv = 1.0f / scalar;

            return Vector2(x * fInv, y * fInv);
        }

        inline Vector2 operator -() const
        {
            return Vector2(-x, -y);
        }

        inline Vector2& operator +=(const Vector2& other)
        {
            x += other.x;
            y += other.y;

            return *this;
        }

        inline Vector2& operator -=( const Vector2& other)
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }

        inline Vector2& operator *=(const float scalar)
        {
            x *= scalar;
            y *= scalar;

            return *this;
        }

        inline Vector2& operator /=(const float scalar)
        {
            assert(scalar != 0.0);

            float fInv = 1.0f / scalar;

            x *= fInv;
            y *= fInv;

            return *this;
        }

        inline float length () const
        {
			return Math::sqrt(x * x + y * y);
        }

        inline float lengthSquared() const
        {
            return x * x + y * y;
        }

        inline float distance(const Vector2& other) const
        {
            return (*this - other).length();
        }

        inline float distanceSquared(const Vector2& other) const
        {
            return (*this - other).lengthSquared();
        }

        inline float dot(const Vector2& vec) const
        {
            return x * vec.x + y * vec.y;
        }

        inline float normalise()
        {
            float fLength = Math::sqrt(x * x + y * y);

            if (fLength > 1e-08)
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
            }

            return fLength;
        }

        inline Vector2 midPoint(const Vector2& vec) const
        {
            return Vector2((x + vec.x) * 0.5f, (y + vec.y) * 0.5f);
        }

        inline bool operator <(const Vector2& other) const
        {
            if(x < other.x && y < other.y)
                return true;
            return false;
        }

        inline bool operator > (const Vector2& other) const
        {
            if( x > other.x && y > other.y )
                return true;
            return false;
        }

        inline Vector2 perpendicular(void) const
        {
            return Vector2 (-y, x);
        }

        inline float crossProduct(const Vector2& other) const
        {
            return x * other.y - y * other.x;
        }

        inline bool isZeroLength() const
        {
            float sqlen = (x * x) + (y * y);
            return (sqlen < (1e-06 * 1e-06));
        }

        inline Vector2 normalised() const
        {
            Vector2 ret = *this;
            ret.normalise();
            return ret;
        }

        inline Vector2 reflect(const Vector2& normal) const
        {
            return Vector2(*this - (normal * (2 * this->dot(normal))));
        }

		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y);
		}

        static const Vector2 ZERO;
		static const Vector2 ONE;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
	};
}

#endif
