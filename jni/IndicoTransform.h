
#ifndef _INDICOTRANSFORM_H_
#define _INDICOTRANSFORM_H_

#include "IndicoMatrix.h"
#include "IndicoQuaternion.h"
#include "IndicoVector3.h"

namespace Indico
{
	class Transform
	{
	private:
		Quaternion mOrientation;
		Vector3 mTranslation;
		Vector3 mScale;
		Matrix mMat;
		Matrix mInverseMat;

	public:
		Transform(const Quaternion &orientation, const Vector3 &translation, const Vector3 &scale) : mOrientation(orientation), mTranslation(translation), mScale(scale)
		{
			update();
		}

		~Transform() { }

		inline Quaternion getOrientation() const
		{
			return mOrientation;
		}

		inline Vector3 getTranslation() const
		{
			return mTranslation;
		}

		inline Vector3 getScale() const
		{
			return mScale;
		}

		inline Matrix getMat() const
		{
			return mMat;
		}

		inline Matrix getInverseMat() const
		{
			return mInverseMat;
		}

		inline void setOrientation(const Quaternion& orientation)
		{
			mOrientation = orientation;
		}

		inline void setTranslation(const Vector3& translation)
		{
			mTranslation = translation;
		}

		inline void setScale(const Vector3& scale)
		{
			mScale = scale;
		}

		void update()
		{
			mOrientation.normalise();

			mMat =
				Matrix::createScale(mScale) *
				Matrix(mOrientation) *
				Matrix::createTranslation(mTranslation);

			Vector3 invscale(1.0f / mScale.x, 1.0f / mScale.y, 1.0f / mScale.z);

			mInverseMat =
				Matrix::createTranslation(-mTranslation) *
				Matrix(mOrientation.conjugate()) *
				Matrix::createScale(invscale);
		}
	};
}

#endif
