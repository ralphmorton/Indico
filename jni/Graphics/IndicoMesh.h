
#ifndef _INDICOMESH_H_
#define _INDICOMESH_H_

#include "IndicoResource.h"
#include "IndicoTypeInfo.h"
#include "Graphics/IndicoGL.h"
#include "Graphics/IndicoVertexLayout.h"

namespace Indico
{
	namespace Graphics
	{
		class Surface
		{
		private:
			friend class RenderSystem;

		private:
			GLuint mBuf;
			unsigned int mNum;
			string mName;

		private:
			Surface(const Surface& other) = delete;
			Surface& operator = (const Surface& other) = delete;

		public:
			Surface(unsigned int num, const unsigned int* data, const string& name);
			~Surface();

			inline GLuint buf() const { return mBuf; }
			inline unsigned int indexCount() const { return mNum; }
			inline const string& name() const { return mName; }
		};

		class Mesh : public Resource
		{
		private:
			friend class RenderSystem;

		private:
			GLuint mBuf;
			unsigned int mVertSize;
			unsigned int mVertCount;
			unsigned int mVboSize;
			VertexLayout mLayout;
			Vector<Surface*> mSurfaces;

		private:
			Mesh(const Mesh& other) = delete;
			Mesh& operator = (const Mesh& other) = delete;

		public:
			Mesh(unsigned int vertSize, unsigned int vertCount, const void* vboData, const VertexLayout& layout);
			~Mesh();

			unsigned int numSurfaces() const;

			void addSurface(unsigned int numIndices, const unsigned int* indexData, const string& name);

			const Surface& operator [](unsigned int ix) const;

			inline GLuint buf() const { return mBuf; }
			inline unsigned int vertSize() const { return mVertSize; }
			inline unsigned int vertCount() const { return mVertCount; }
			inline unsigned int vboSize() const { return mVboSize; }
			inline const VertexLayout& layout() const { return mLayout; }

			TYPED_DECL();
		};
	}
}

#endif
