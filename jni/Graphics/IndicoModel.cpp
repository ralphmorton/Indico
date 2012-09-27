
#include "Graphics/IndicoModel.h"

namespace Indico
{
	namespace Graphics
	{
		Model::Model(const Mesh& mesh, const Skin& skin) : mMesh(mesh), mSkin(skin), mTransform(Matrix::IDENTITY) { }
		Model::~Model() { }

		const Matrix& Model::getTransform() const
		{
			return mTransform;
		}

		void Model::setTransform(const Matrix& transform)
		{
			mTransform = transform;
		}

		const Mesh& Model::mesh() const
		{
			return mMesh;
		}

		const Skin& Model::skin() const
		{
			return mSkin;
		}
	}
}
