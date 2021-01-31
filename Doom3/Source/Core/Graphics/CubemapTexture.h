#pragma once
#include "Texture.h"

namespace doom
{
	namespace graphics
	{
		class CubemapTexture : public Texture
		{
		public:

			CubemapTexture(eTextureType textureType,
				eTargetTexture target, eInternalFormat internalFormat, unsigned int width, eDataFormat format, eDataType type, const void* data);
			CubemapTexture(eTextureType textureType,
				eTargetTexture target, eInternalFormat internalFormat, unsigned int width, unsigned int height, eDataFormat format, eDataType type, const void* data);

			inline void TexImage1D(int level, eInternalFormat internalformat, int width, eDataFormat format, eDataType type, const void* data) noexcept override
			{
				this->BindTexture();
				for (unsigned int i = 0; i < 6; i++)
				{
					glTexImage1D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, static_cast<unsigned int>(internalformat), width, 0, static_cast<unsigned int>(format), static_cast<unsigned int>(type), data);
				}
			}

			inline void TexImage1D(int face, int level, eInternalFormat internalformat, int width, eDataFormat format, eDataType type, const void* data) noexcept
			{
				this->BindTexture();
				glTexImage1D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, static_cast<unsigned int>(internalformat), width, 0, static_cast<unsigned int>(format), static_cast<unsigned int>(type), data);
			}


			inline void TexImage2D(int level, eInternalFormat internalformat, int width, int height, eDataFormat format, eDataType type, const void* data) noexcept override
			{
				this->BindTexture();
				for (unsigned int i = 0; i < 6; i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, static_cast<unsigned int>(internalformat), width, height, 0, static_cast<unsigned int>(format), static_cast<unsigned int>(type), data);
				}
			}

			inline void TexImage2D(int face, int level, eInternalFormat internalformat, int width, int height, eDataFormat format, eDataType type, const void* data) noexcept
			{
				this->BindTexture();
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, static_cast<unsigned int>(internalformat), width, height, 0, static_cast<unsigned int>(format), static_cast<unsigned int>(type), data);
			}

		};
	}
}