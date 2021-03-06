#include "FrameBuffer.h"
#include "../Graphics_Server.h"



using namespace doom::graphics;


doom::graphics::FrameBuffer::FrameBuffer()
{
}

FrameBuffer::FrameBuffer(unsigned int defaultWidth, unsigned int defaultHeight)
	: mDefaultWidth{ defaultWidth }, mDefaultHeight{ defaultHeight }, mClearBit{ 0 }
{
	mAttachedRenderBuffers.reserve(RESERVED_RENDERBUFFER_COUNT);
	mAttachedColorTextures.reserve(RESERVED_COLOR_TEXTURE_COUNT);
	mAttachedDepthTextures.reserve(RESERVED_DEPTH_TEXTURE_COUNT);
	mAttachedDepthStencilTextures.reserve(RESERVED_DEPTH_STENCIL_TEXTURE_COUNT);
}

void doom::graphics::FrameBuffer::GenerateBuffer(unsigned int defaultWidth, unsigned int defaultHeight)
{
	this->mDefaultWidth = defaultWidth;
	this->mDefaultHeight = defaultHeight;
	glGenFramebuffers(1, &(this->mFrameBufferID));
}

void FrameBuffer::BindFrameBuffer(FrameBuffer* frameBuffer)
{
	if (D_OVERLAP_BIND_CHECK_CHECK_IS_NOT_BOUND_AND_BIND_ID(FRAMEBUFFER_TAG, ((frameBuffer != nullptr) ? frameBuffer->mFrameBufferID.Get() : 0)) )
	{
		FrameBuffer::PreviousFrameBuffer = CurrentFrameBuffer;
		if (frameBuffer == nullptr)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, graphics::Graphics_Server::GetScreenWidth(), graphics::Graphics_Server::GetScreenHeight());
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->mFrameBufferID);
			glViewport(0, 0, frameBuffer->mDefaultWidth, frameBuffer->mDefaultHeight);
		}
		FrameBuffer::CurrentFrameBuffer = frameBuffer;
	}
}

FrameBuffer::~FrameBuffer()
{
	if (this->mFrameBufferID != 0)
	{
		glDeleteFramebuffers(1, &(this->mFrameBufferID));
	}
	
}




void FrameBuffer::CheckIsFrameBufferSuccesfullyCreated() noexcept
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{//Fail Creating FrameBuffer
		D_DEBUG_LOG({ "fail frame buffer", std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)) }, logger::eLogType::D_ERROR);
	}
}

const doom::graphics::SingleTexture& FrameBuffer::GetFrameBufferTexture(GraphicsAPI::eBufferType bufferType, unsigned int index) const
{
	switch (bufferType)
	{
	case GraphicsAPI::eBufferType::COLOR:
		return this->mAttachedColorTextures[index];
		break;

	case GraphicsAPI::eBufferType::DEPTH:
		return this->mAttachedDepthTextures[index];
		break;

	case GraphicsAPI::eBufferType::DEPTH_STENCIL:
		return this->mAttachedDepthStencilTextures[index];
		break;
	default:
		NEVER_HAPPEN;
	}
}

doom::graphics::SingleTexture& FrameBuffer::GetFrameBufferTexture(GraphicsAPI::eBufferType bufferType, unsigned int index)
{
	switch (bufferType)
	{
	case GraphicsAPI::eBufferType::COLOR:
		return this->mAttachedColorTextures[index];
		break;

	case GraphicsAPI::eBufferType::DEPTH:
		return this->mAttachedDepthTextures[index];
		break;

	case GraphicsAPI::eBufferType::DEPTH_STENCIL:
		return this->mAttachedDepthStencilTextures[index];
		break;
	default:
		NEVER_HAPPEN;
	}
}

bool doom::graphics::FrameBuffer::IsGenerated()
{
	return this->mFrameBufferID != 0;
}

RenderBuffer& FrameBuffer::AttachRenderBuffer(GraphicsAPI::eBufferType renderBufferType, unsigned int width, unsigned int height)
{
	D_ASSERT(this->mFrameBufferID != 0);
	auto& createdRenderBuffer = this->mAttachedRenderBuffers.emplace_back(*this, renderBufferType, width, height);
	this->mClearBit |= static_cast<unsigned int>(renderBufferType);

	FrameBuffer::CheckIsFrameBufferSuccesfullyCreated();
	return createdRenderBuffer;
}

SingleTexture& FrameBuffer::AttachTextureBuffer(GraphicsAPI::eBufferType frameBufferType, unsigned int width, unsigned int height)
{
	D_ASSERT(this->mFrameBufferID != 0);

	this->BindFrameBuffer();

	
	switch (frameBufferType)
	{
	case GraphicsAPI::eBufferType::COLOR:
	{
		SingleTexture colorTexture{ Texture::eTextureType::DIFFUSE, Texture::eTargetTexture::TEXTURE_2D,
			eTextureInternalFormat::RGBA16F, width, height, eTextureComponentFormat::RGBA, Texture::eDataType::FLOAT, NULL };
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<unsigned int>(this->mAttachedColorTextures.size()), static_cast<unsigned int>(Texture::eBindTarget::TEXTURE_2D), colorTexture.GetID(), 0);

		this->mClearBit |= static_cast<unsigned int>(GraphicsAPI::eBufferType::COLOR);
		this->mDrawTarget |= GL_COLOR_ATTACHMENT0 + this->mAttachedColorTextures.size();

		auto& createdTexture = this->mAttachedColorTextures.emplace_back(std::move(colorTexture));

		std::vector<unsigned int> drawBufferTarget{};
		for (unsigned int i = 0; i < this->mAttachedColorTextures.size(); i++)
		{
			drawBufferTarget.emplace_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers(static_cast<int>(this->mAttachedColorTextures.size()), drawBufferTarget.data());
		return createdTexture;
		break;
	}
	case GraphicsAPI::eBufferType::DEPTH:
	{
		D_ASSERT(this->mAttachedDepthTextures.size() == 0);

		SingleTexture depthTexture{ Texture::eTextureType::DIFFUSE, Texture::eTargetTexture::TEXTURE_2D,
			eTextureInternalFormat::DEPTH_COMPONENT, width, height, eTextureComponentFormat::DEPTH_COMPONENT, Texture::eDataType::FLOAT, NULL };
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, static_cast<unsigned int>(Texture::eBindTarget::TEXTURE_2D), depthTexture.GetID(), 0);

		this->mAttachedDepthTextures.push_back(std::move(depthTexture));
		auto& createdDepthTexture = this->mAttachedDepthTextures.back();

		this->mClearBit |= static_cast<unsigned int>(GraphicsAPI::eBufferType::DEPTH);

		return createdDepthTexture;
		break;
	}
	case GraphicsAPI::eBufferType::DEPTH_STENCIL:
	{
		D_ASSERT(this->mAttachedDepthStencilTextures.size() == 0);

		SingleTexture depthStencilTexture{ Texture::eTextureType::DIFFUSE, Texture::eTargetTexture::TEXTURE_2D,
			eTextureInternalFormat::DEPTH24_STENCIL8, width, height, eTextureComponentFormat::DEPTH_STENCIL, Texture::eDataType::UNSIGNED_INT_24_8, NULL };
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, static_cast<unsigned int>(Texture::eBindTarget::TEXTURE_2D), depthStencilTexture.GetID(), 0);

		this->mAttachedDepthStencilTextures.push_back(std::move(depthStencilTexture));
		auto& createdDepthStencilTexture = this->mAttachedDepthStencilTextures.back();

		this->mClearBit |= static_cast<unsigned int>(GraphicsAPI::eBufferType::DEPTH_STENCIL);
		return createdDepthStencilTexture;
		break;

	}
		
	default:
		NEVER_HAPPEN;
		break;

	}
	
}



		
		
