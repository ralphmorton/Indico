
#ifndef _INDICOQUATERNION_H_
#define _INDICOQUATERNION_H_

#include "IndicoMath.h"
#include "IndicoVector3.h"

#include <assert.h>

namespace Indico
{
	class Quaternion
	{
	public:
		float w,x,y,z;

	public:
		inline Quaternion(float fW = 1.0, float fX = 0.0, float fY = 0.0, float fZ = 0.0) : w(fW), x(fX), y(fY), z(fZ) {}
        inline Quaternion(const Vector3& axis, const float& angle)
        {
            float fHalfAngle(0.5f*angle);
	        float fSin = Math::sin(fHalfAngle);
	        w = Math::cos(fHalfAngle);
	        x = fSin*axis.x;
	        y = fSin*axis.y;
	        z = fSin*axis.z;
        }

		inline Quaternion operator +(const Quaternion& rkQ) const
		{
			return Quaternion(w+rkQ.w,x+rkQ.x,y+rkQ.y,z+rkQ.z);
		}

        inline Quaternion operator -(const Quaternion& rkQ) const
		{
			return Quaternion(w-rkQ.w,x-rkQ.x,y-rkQ.y,z-rkQ.z);
		}

        inline Quaternion operator *(const Quaternion& rkQ) const
		{
			return Quaternion
	        (
	            w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
	            w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
	            w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
	            w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
	        );
		}

        inline Quaternion operator *(float scalar) const
		{
			return Quaternion(scalar*w,scalar*x,scalar*y,scalar*z);
		}

        inline Quaternion operator -() const
		{
			return Quaternion(-w,-x,-y,-z);
		}

        inline bool operator ==(const Quaternion& other) const
		{
			return (other.x == x) && (other.y == y) && (other.z == z) && (other.w == w);
		}

        inline bool operator !=(const Quaternion& other) const
		{
			return !operator==(other);
		}

        inline Quaternion& operator =(const Quaternion& other)
		{
			w = other.w;
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

        inline Vector3 xAxis() const
		{
			float fTy  = 2.0f*y;
	        float fTz  = 2.0f*z;
	        float fTwy = fTy*w;
	        float fTwz = fTz*w;
	        float fTxy = fTy*x;
	        float fTxz = fTz*x;
	        float fTyy = fTy*y;
	        float fTzz = fTz*z;

	        return Vector3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
		}

        inline Vector3 yAxis() const
		{
			float fTx  = 2.0f*x;
	        float fTy  = 2.0f*y;
	        float fTz  = 2.0f*z;
	        float fTwx = fTx*w;
	        float fTwz = fTz*w;
	        float fTxx = fTx*x;
	        float fTxy = fTy*x;
	        float fTyz = fTz*y;
	        float fTzz = fTz*z;

	        return Vector3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
		}

        inline Vector3 zAxis() const
		{
			float fTx  = 2.0f*x;
	        float fTy  = 2.0f*y;
	        float fTz  = 2.0f*z;
	        float fTwx = fTx*w;
	        float fTwy = fTy*w;
	        float fTxx = fTx*x;
	        float fTxz = fTz*x;
	        float fTyy = fTy*y;
	        float fTyz = fTz*y;

	        return Vector3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
		}

		inline float dot(const Quaternion& other) const
		{
			return w*other.w+x*other.x+y*other.y+z*other.z;
		}

		inline void toAxisAngle(Vector3& axis, float& angle) const
		{
			float fSqrLength = x*x+y*y+z*z;
	        if ( fSqrLength > 0.0 )
	        {
	            angle = 2.0f*Math::acos(w);
	            float fInvLength = 1.0f / Math::sqrt(fSqrLength);
	            axis.x = x*fInvLength;
	            axis.y = y*fInvLength;
	            axis.z = z*fInvLength;
	        }
	        else
	        {
	            angle = 0.0f;
	            axis.x = 1.0f;
	            axis.y = 0.0f;
	            axis.z = 0.0f;
	        }
		}

        inline float normalise()
		{
			float len = w*w+x*x+y*y+z*z;
	        float factor = 1.0f / Math::sqrt(len);
	        *this = *this * factor;
	        return len;
		}

        inline Quaternion inverse() const
		{
			float fNorm = w*w+x*x+y*y+z*z;
	        if ( fNorm > 0.0 )
	        {
	            float fInvNorm = 1.0f/fNorm;
	            return Quaternion(w*fInvNorm,-x*fInvNorm,-y*fInvNorm,-z*fInvNorm);
	        }
	        else
	            return ZERO;
		}

		inline Quaternion conjugate() const
		{
			return Quaternion(w, -x, -y, -z);
		}

		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}

        static const Quaternion ZERO;
        static const Quaternion IDENTITY;
	};

	inline Vector3 operator *(const Vector3& v, const Quaternion& q)
    {
        Vector3 uv, uuv;
		Vector3 qvec(q.x, q.y, q.z);
		uv = qvec.cross(v);
		uuv = qvec.cross(uv);
		uv *= (2.0f * q.w);
		uuv *= 2.0f;

		return v + uv + uuv;
    }
}

#endif
