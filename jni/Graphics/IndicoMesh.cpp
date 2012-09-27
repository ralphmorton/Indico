
#include "Graphics/IndicoMesh.h"

namespace Indico
{
	namespace Graphics
	{
		Surface::Surface(unsigned int num, const unsigned int* data, const string& name) : mNum(num), mName(name)
		{
			glGenBuffers(1, &mBuf);

			if (!mBuf) throw Exception("Failed to create buffer");

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuf);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNum*sizeof(int), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
				throw Exception("Could not create surface index buffer");
		}

		Surface::~Surface()
		{
			glDeleteBuffers(1, &mBuf);
		}

		Mesh::Mesh(unsigned int vertSize, unsigned int vertCount, const void* vboData, const VertexLayout& layout) : mVertSize(vertSize), mVertCount(vertCount), mVboSize(vertSize*vertCount), mLayout(layout)
		{
			glGenBuffers(1, &mBuf);

			if (!mBuf) throw Exception("Failed to create buffer");

			glBindBuffer(GL_ARRAY_BUFFER, mBuf);
			glBufferData(GL_ARRAY_BUFFER, mVboSize, vboData, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
				throw Exception("Could not create mesh vertex buffer");
		}

		Mesh::~Mesh()
		{
			glDeleteBuffers(1, &mBuf);
		}

		unsigned int Mesh::numSurfaces() const
		{
			return mSurfaces.size();
		}

		void Mesh::addSurface(unsigned int numIndices, const unsigned int* indexData, const string& name)
		{
			Surface* s = new Surface(numIndices, indexData, name);
			mSurfaces.add(s);
		}

		const Surface& Mesh::operator [](unsigned int ix) const
		{
			return *mSurfaces[ix];
		}

		TYPED_DEF(Graphics::Mesh);
	}
}
