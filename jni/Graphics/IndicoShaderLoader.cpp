
#include "Graphics/IndicoShaderLoader.h"

#include <string.h>

namespace Indico
{
	namespace Graphics
	{
		shared_ptr<Resource> VertexShaderLoader::load(ResourceStream& stream)
		{
			char buf[257];

			string data;

			while (!stream.eos())
			{
				memset(buf, 0, 257);
				stream.read(buf, 256);
				data += buf;
			}

			Resource* shader = new VertexShader(data.c_str());

			return shared_ptr<Resource>(shader);
		}

		const TypeInfo& VertexShaderLoader::loadType() const { return VertexShader::type_s(); }

		shared_ptr<Resource> PixelShaderLoader::load(ResourceStream& stream)
		{
			char buf[257];

			string data;

			while (!stream.eos())
			{
				memset(buf, 0, 257);
				stream.read(buf, 256);
				data += buf;
			}

			Resource* shader = new PixelShader(data.c_str());

			return shared_ptr<Resource>(shader);
		}

		const TypeInfo& PixelShaderLoader::loadType() const { return PixelShader::type_s(); }
	}
}
