
#ifndef _INDICOVERTEXLAYOUT_H_
#define _INDICOVERTEXLAYOUT_H_

#include "IndicoVector.h"

namespace Indico
{
	namespace Graphics
	{
		enum VertexElementType
		{
			VERTEXELEMENT_FLOAT1,
			VERTEXELEMENT_FLOAT2,
			VERTEXELEMENT_FLOAT3,
			VERTEXELEMENT_FLOAT4
		};

		class VertexElement
		{
		private:
			VertexElementType mType;
			unsigned short mStride;
			unsigned short mOffset;

		public:
			VertexElement(VertexElementType type, unsigned short stride, unsigned short offset) : mType(type), mStride(stride), mOffset(offset) { }

			inline VertexElementType type() const { return mType; }
			inline unsigned short stride() const { return mStride; }
			inline unsigned short offset() const { return mOffset; }
		};

		class VertexLayout
		{
		private:
			Vector<VertexElement> mElements;

		public:
			unsigned int numElements() const;

			void add(const VertexElement& element);
			void remove(unsigned int ix);

			VertexElement& operator [](unsigned int ix);
			const VertexElement& operator [](unsigned int ix) const;
		};
	}
}

#endif
