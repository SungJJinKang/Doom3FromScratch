#include "Renderer.h"
#include "../Core/Graphics/Material.h"
#include <Transform.h>

void doom::Renderer::InitComponent()
{
	RendererComponentStaticIterator::AddRendererToStaticContainer();

	this->mEntityBlockViewer = graphics::Graphics_Server::GetSingleton()->mCullingSystem->AllocateNewEntity(this);

	this->AddLocalDirtyToTransformDirtyReceiver(BVH_Sphere_Node_Object::IsWorldColliderCacheDirty);
	this->AddLocalDirtyToTransformDirtyReceiver(ColliderUpdater<doom::physics::AABB3D>::IsWorldColliderCacheDirty);

	//this->BVH_Sphere_Node_Object::UpdateWorldBVhColliderCache(true);
	
	//BVH_Sphere_Node_Object::InsertBVHLeafNode(graphics::Graphics_Server::GetSingleton()->mCullDistance., BVH_Sphere_Node_Object::GetWorldBVhColliderCacheByReference(), nullptr);
	//BVH_AABB3D_Node_Object::InsertBVHLeafNode(graphics::Graphics_Server::GetSingleton()->mViewFrustumCulling.mBVHSphere, BVH_AABB3D_Node_Object:::GetWorldBVhColliderCacheByReference(), nullptr);
}

const math::Matrix4x4& doom::Renderer::GetModelMatrix()
{
	return this->GetTransform()->GetModelMatrix();
}

void doom::Renderer::OnDestroy()
{
	graphics::Graphics_Server::GetSingleton()->mCullingSystem->RemoveEntityFromBlock(this->mEntityBlockViewer);
}

doom::Renderer::Renderer() : ServerComponent(), StaticContainer(), mTargetMaterial{}
{

}


void doom::Renderer::MergeBVHBitFlag()
{
	//this->mRenderingBitFlag |= graphics::eRenderingBitflag::IsVisible & ( BVH_Sphere_Node_Object::mBVH_Node_View->GetBitFlag() | BVH_AABB3D_Node_Object::mBVH_Node_View->GetBitFlag() );
}

void doom::Renderer::ClearRenderingBitFlag()
{
	this->mRenderingBitFlag = 0;
}

void doom::Renderer::SetMaterial(graphics::Material* material) noexcept
{
	this->mTargetMaterial = material;
}

/*
const doom::physics::Sphere& doom::Renderer::GetBoudingSphere()
{
	this->UpdateWorldBVhColliderCache(false);
	if (mIsBoundingSphereDirty.GetIsDirty(true) == true)
	{
		this->mBoundingSphere = this->GetWorldBVhColliderCacheByReference();
	}
	return this->mBoundingSphere;
}
*/

void doom::Renderer::SetMaterial(graphics::Material& material) noexcept
{
	this->SetMaterial(&material);
}

