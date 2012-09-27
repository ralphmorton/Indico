
#ifndef _INDICOMODEL_H_
#define _INDICOMODEL_H_

#include "IndicoMatrix.h"
#include "Graphics/IndicoMesh.h"
#include "Graphics/IndicoSkin.h"

namespace Indico
{
	namespace Graphics
	{
		class Model
		{
		private:
			const Mesh& mMesh;
			const Skin& mSkin;
			Matrix mTransform;

		public:
			Model(const Mesh& mesh, const Skin& skin);
			~Model();

			const Matrix& getTransform() const;
			void setTransform(const Matrix& transform);

			const Mesh& mesh() const;
			const Skin& skin() const;
		};
	}
}

#endif
