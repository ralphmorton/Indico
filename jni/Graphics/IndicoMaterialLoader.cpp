
#include "Graphics/IndicoMaterialLoader.h"
#include "IndicoEngine.h"

namespace Indico
{
	namespace Graphics
	{
		shared_ptr<Resource> MaterialLoader::load(ResourceStream& stream)
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

			Material* mat = new Material;
			shared_ptr<Resource> res(mat);

			int p = data.find("\n");
			while (p != -1)
			{
				string line = data.substr(0, p).trim();
				data = data.substr(p+1);
				p = data.find("\n");

				if (!line.length())
					continue;

				int cp = line.find(":");
				if (cp == -1)
					throw Exception("Malformed material parameter: " + line);

				string type = line.substr(0, cp).trim().toLower();
				line = line.substr(cp+1);

				int ep = line.find("=");
				if (ep == -1)
					throw Exception("Malformed material parameter: " + type+":"+line);

				string id = line.substr(0, ep).trim();
				string val = line.substr(ep+1).trim().trimNewlines();

				if (type == "texture")
				{
					Vector<string> opts;
					int sp = val.find(" ");

					string tval;

					if (sp == -1)
					{
						tval = val;
						val = "";
					}
					else
					{
						tval = val.substr(0, sp);
						val = val.substr(sp+1);
						sp = val.find(" ");
					}

					while (sp != -1)
					{
						string opt = val.substr(0, sp).trim();
						val = val.substr(sp+1);
						sp = val.find(" ");

						if (opt.length())
							opts.add(opt);
					}

					if (val.length())
						opts.add(val);

					Texture* tex = rman.get<Graphics::Texture>(tval);

					glBindTexture(GL_TEXTURE_2D, tex->tex());
					for (unsigned int ox = 0; ox < opts.size(); ox++)
					{
						string opt = opts[ox];
						if (opt == "clamp_s")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_CLAMP);
						else if (opt == "clamp_t")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_CLAMP);
						else if (opt == "repeat_s")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_REPEAT);
						else if (opt == "repeat_t")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_REPEAT);
						else if (opt == "clampedge_s")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_CLAMPEDGE);
						else if (opt == "clampedge_t")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_CLAMPEDGE);
						else if (opt == "mag_nearest")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						else if (opt == "mag_linear")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						else if (opt == "min_nearest")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						else if (opt == "min_linear")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						else if (opt == "min_linear_mipmap_nearest")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
						else if (opt == "min_nearest_mipmap_linear")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						else if (opt == "min_linear_mipmap_linear")
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						else
							throw Exception("Unknown texture option '" + opt + "'");
					}
					glBindTexture(GL_TEXTURE_2D, 0);

					GLuint error = glGetError();
					if (error != GL_NO_ERROR)
						throw Exception("Could not set filtering and wrap options for texture: " + id);

					mat->set(id, tex);
				}
				else if (type == "float")
				{
					mat->set(id, val.tof());
				}
				else if (type == "vec2")
				{
					int sp = val.find(" ");
					if (sp == -1)
						throw Exception("Malformed material vec2 parameter: " + id);

					float v1 = val.substr(0,sp).tof();
					float v2 = val.substr(sp+1).tof();

					mat->set(id, Vector2(v1, v2));
				}
				else if (type == "vec3")
				{
					int sp = val.find(" ");
					if (sp == -1)
						throw Exception("Malformed material vec3 parameter: " + id);

					float v1 = val.substr(0,sp).tof();

					val = val.substr(sp+1);
					sp = val.find(" ");
					if (sp == -1)
						throw Exception("Malformed material vec3 parameter: " + id);

					float v2 = val.substr(0,sp).tof();
					float v3 = val.substr(sp+1).tof();

					mat->set(id, Vector3(v1, v2, v3));
				}
				else if (type == "vec4")
				{
					int sp = val.find(" ");
					if (sp == -1)
						throw Exception("Malformed material vec4 parameter: " + id);

					float v1 = val.substr(0,sp).tof();

					val = val.substr(sp+1);
					sp = val.find(" ");
					if (sp == -1)
						throw Exception("Malformed material vec4 parameter: " + id);

					float v2 = val.substr(0,sp).tof();

					val = val.substr(sp+1);
					sp = val.find(" ");
					if (sp == -1)
						throw Exception("Malformed material vec4 parameter: " + id);

					float v3 = val.substr(0,sp).tof();
					float v4 = val.substr(sp+1).tof();

					mat->set(id, Vector4(v1, v2, v3, v4));
				}
				else
					throw Exception("Unknown material parameter type '" + type + "'");
			}

			return res;
		}

		const TypeInfo& MaterialLoader::loadType() const { return Material::type_s(); }
	}
}


















