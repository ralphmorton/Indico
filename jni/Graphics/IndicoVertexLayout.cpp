
#include "Graphics/IndicoVertexLayout.h"

namespace Indico
{
	namespace Graphics
	{
		unsigned int VertexLayout::numElements() const
		{
			return mElements.size();
		}

		void VertexLayout::add(const VertexElement& element)
		{
			mElements.add(element);
		}

		void VertexLayout::remove(unsigned int ix)
		{
			mElements.remove(ix);
		}

		VertexElement& VertexLayout::operator [](unsigned int ix)
		{
			return mElements[ix];
		}

		const VertexElement& VertexLayout::operator [](unsigned int ix) const
		{
			return mElements[ix];
		}
	}
}
