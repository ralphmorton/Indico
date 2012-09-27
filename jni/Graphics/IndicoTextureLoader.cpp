
#include "Graphics/IndicoTextureLoader.h"

#include "libpng/png.h"

void png_readcustom(png_structp pptr, png_bytep data, png_size_t length)
{
	Indico::ResourceStream* stream = (Indico::ResourceStream*)png_get_io_ptr(pptr);
	stream->read((void*)data, length);
}

namespace Indico
{
	namespace Graphics
	{
		shared_ptr<Resource> TextureLoader::load(ResourceStream& stream)
		{
			try
			{
				unsigned char buf[8];
				stream.read(buf, 8);

				int invalid = png_sig_cmp(buf, 0, 8);
				if (invalid)
					throw Exception("Could not load texture: Invalid png header");

				png_structp pptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
				if (!pptr)
					throw Exception("Could not load texture: Could not create read struct");

				png_infop iptr = png_create_info_struct(pptr);
				if (!iptr)
				{
					png_destroy_read_struct(&pptr, (png_infopp)0, (png_infopp)0);
					throw Exception("Could not load texture: Could not create info struct");
				}

				png_infop eptr = png_create_info_struct(pptr);
				if (!eptr)
				{
					png_destroy_read_struct(&pptr, &iptr, (png_infopp)0);
					throw Exception("Could not load texture: Could not create end struct");
				}

				if (setjmp(png_jmpbuf(pptr)))
				{
					png_destroy_read_struct(&pptr, &iptr, &eptr);
					throw Exception("Could not load texture: Could not setjmp to libpng handling code");
				}

				png_set_read_fn(pptr, &stream, png_readcustom);
				png_set_sig_bytes(pptr, 8);
				png_read_info(pptr, iptr);

				int depth, type;
				png_uint_32 width, height;
				png_get_IHDR(pptr, iptr, &width, &height, &depth, &type, 0, 0, 0);
				png_uint_32 channels = png_get_channels(pptr, iptr);

				switch (type)
				{
				case PNG_COLOR_TYPE_PALETTE:
					png_set_palette_to_rgb(pptr);
					channels = 3;
					break;
				case PNG_COLOR_TYPE_GRAY:
					if (depth < 8)
						png_set_expand_gray_1_2_4_to_8(pptr);
					depth = 8;
					break;
				}

				if (png_get_valid(pptr, iptr, PNG_INFO_tRNS))
				{
					png_set_tRNS_to_alpha(pptr);
					channels += 1;
				}

				if (depth == 16)
					png_set_strip_16(pptr);

				png_read_update_info(pptr, iptr);

				int rowlength = png_get_rowbytes(pptr, iptr);
				png_byte *data = new png_byte[rowlength * height];
				png_bytep *rows = new png_bytep[height];

				for (int i = -1; ++i < height;)
					rows[height - 1 - i] = data + i * rowlength;

				png_read_image(pptr, rows);

				GLuint tex;
				glGenTextures(1, &tex);
				glBindTexture(GL_TEXTURE_2D, tex);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

				int gformat = channels == 3 ? GL_RGB : GL_RGBA;
				glTexImage2D(GL_TEXTURE_2D, 0, gformat, width, height, 0, gformat, GL_UNSIGNED_BYTE, data);

				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_REPEAT);
				glBindTexture(GL_TEXTURE_2D, 0);

				png_destroy_read_struct(&pptr, &iptr, &eptr);
				delete [] rows;
				delete [] data;

				int error = glGetError();
				if (error != GL_NO_ERROR)
					throw Exception("Could not load texture: Failed to create OpenGL texture");

				return shared_ptr<Resource>(new Texture(WRAP_REPEAT, WRAP_REPEAT, width, height, tex));
			}
			catch (Exception& ex)
			{
				throw ex;
			}
		}

		const TypeInfo& TextureLoader::loadType() const { return Texture::type_s(); }
	}
}





















