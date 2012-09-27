
#include "Graphics/IndicoMeshLoader.h"

namespace Indico
{
	namespace Graphics
	{
		void MeshLoader::readVertexElement(const string& el, Vector<VertexElementType>& types, Vector<unsigned short>& offsets, unsigned short& numf) const
		{
			offsets.add(numf * sizeof(float));

			if (el == "pos" || el == "norm" || el == "col")
			{
				types.add(VERTEXELEMENT_FLOAT3);
				numf += 3;
			}
			else if (el == "tan")
			{
				types.add(VERTEXELEMENT_FLOAT4);
				numf += 4;
			}
			else if (el == "uv")
			{
				types.add(VERTEXELEMENT_FLOAT2);
				numf += 2;
			}
			else
				throw Exception("Unknown vertex element type '" + el + "'");

		}

		unsigned int MeshLoader::readVertexLayout(const string& line, VertexLayout& layout) const
		{
			Vector<VertexElementType> types;
			Vector<unsigned short> offsets;
			unsigned short numf = 0;

			string l = line;
			int p = l.find(" ");

			while (p != -1)
			{
				string vt = l.substr(0, p);
				l = l.substr(p+1);
				p = l.find(" ");

				readVertexElement(vt, types, offsets, numf);
			}

			readVertexElement(l, types, offsets, numf);

			for (unsigned int i = 0; i < types.size(); i++)
				layout.add(VertexElement(types[i], numf * sizeof(float), offsets[i]));

			return numf;
		}

		shared_ptr<Resource> MeshLoader::load(ResourceStream& stream) //TODO: clean this up
		{
			char buf[257];
			string data;

			VertexLayout layout;
			unsigned int vn;
			unsigned int vc, vcc;
			unsigned int vcur = 0;

			float* vdata = 0;
			Mesh* mesh = 0;
			Vector<unsigned int> sdata;
			string sname;

			int stage = 0;

			try
			{
				while (!stream.eos())
				{
					memset(buf, 0, 257);
					stream.read(buf, 256);
					data += buf;

					if (stream.eos())
						data += "\n";

					int p = data.find("\n");
					while (p != -1)
					{
						string line	= data.substr(0, p).trim();

						data = data.substr(p+1);
						p = data.find("\n");

						if (!line.length())
							continue;

						switch (stage)
						{
						case 0:
							vn = readVertexLayout(line, layout);
							stage++;
							break;
						case 1:
							vc = vcc = line.toi();
							vdata = new float[vc*vn];
							stage++;
							break;
						case 2:
							if (vcc > 0)
							{
								int vfound = 0;
								int sp = line.find(" ");
								while (sp != -1 && vcur < (vn*vc) - 1)
								{
									float fval = line.substr(0, sp).tof();
									line = line.substr(sp+1);
									sp = line.find(" ");

									vdata[vcur++] = fval;
									vfound++;
								}
								float fval2 = line.tof();
								vdata[vcur++] = fval2;

								if (++vfound != vn)
									throw Exception("Invalid vertex data");

								vcc--;
								break;
							}
							mesh = new Mesh(vn*sizeof(float), vc, vdata, layout);
							stage++;
						case 3:
							int mp = line.find("surf ");
							if (mp == 0)
							{
								if (sname.length())
									mesh->addSurface(sdata.size(), &sdata[0], sname);

								sname = line.substr(mp+5).trim().trimNewlines();
								sdata = Vector<unsigned int>(100);
							}
							else if (mp == -1)
							{
								int sp2 = line.find(" ");
								if (sp2 == -1) throw Exception("Invalid triangle indices");
								unsigned int ival = (unsigned int)line.substr(0, sp2).toi();
								line = line.substr(sp2+1);
								sdata.add(ival);

								sp2 = line.find(" ");
								if (sp2 == -1) throw Exception("Invalid triangle indices");
								ival = (unsigned int)line.substr(0, sp2).toi();
								line = line.substr(sp2+1);
								sdata.add(ival);

								ival = (unsigned int)line.toi();
								sdata.add(ival);
							}
							else
								throw Exception("Invalid surface definition: " + line);
						}
					}
				}

				if (sname.length())
					mesh->addSurface(sdata.size(), &sdata[0], sname);
			}
			catch (Exception& ex)
			{
				delete [] vdata;
				delete mesh;
				throw ex;
			}

			return shared_ptr<Resource>(mesh);
		}

		const TypeInfo& MeshLoader::loadType() const { return Mesh::type_s(); }
	};
}
