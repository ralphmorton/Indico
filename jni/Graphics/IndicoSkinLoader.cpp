
#include "Graphics/IndicoSkinLoader.h"
#include "IndicoEngine.h"

namespace Indico
{
	namespace Graphics
	{
		shared_ptr<Resource> SkinLoader::load(ResourceStream& stream)
		{
			char buf[257];
			string data;

			while (!stream.eos())
			{
				memset(buf, 0, 257);
				stream.read(buf, 256);
				data += buf;
			}
			data += "\n";

			ResourceManager& rman = Engine::getPtr()->getResourceManager();

			Skin* skin = new Skin;
			shared_ptr<Resource> res(skin);

			int p = data.find("\n");
			while (p != -1)
			{
				string line = data.substr(0, p).trim().trimNewlines();
				data = data.substr(p+1);
				p = data.find("\n");

				if (!line.length())
					continue;

				int sp = line.find("=");
				if (sp == -1)
					throw Exception("Invalid skin parameter: " + line);

				string name = line.substr(0, sp).trim().trimNewlines();
				string matn = line.substr(sp+1).trim().trimNewlines();

				(*skin)[name] = rman.get<Material>(matn);
			}

			return res;
		}

		const TypeInfo& SkinLoader::loadType() const { return Skin::type_s(); }
	}
}
