#include "Camera.h"
#include "../Core/Scene/Scene.h"
#include "../Core/Math/LightMath_Cpp/Matrix_utility.h"
#include "../Core/Math/LightMath_Cpp/Vector3.h"
#include "Transform.h"
#include "../Graphics/Graphics_Server.h"

using namespace doom;

void Camera::SetProjectionMode(eProjectionType value)
{
	this->mProjectionMode = value;
	this->SetDirtyTrueAtThisFrame();
	this->bmIsProjectionMatrixDirty = true;
	this->bmIsViewMatrixDirty = true;
	this->bmIsViewProjectionMatrixDirty = true;
}

void Camera::SetFieldOfView(float value)
{
	this->mFieldOfViewInDegree = value;
	this->SetDirtyTrueAtThisFrame();
	this->bmIsProjectionMatrixDirty = true;
	this->bmIsViewMatrixDirty = true;
	this->bmIsViewProjectionMatrixDirty = true;
}

void Camera::SetClippingPlaneNear(float value)
{
	this->mClippingPlaneNear = value;
	this->SetDirtyTrueAtThisFrame();
	this->bmIsProjectionMatrixDirty = true;
	this->bmIsViewMatrixDirty = true;
	this->bmIsViewProjectionMatrixDirty = true;
}

void Camera::SetClippingPlaneFar(float value)
{
	this->mClippingPlaneFar = value;
	this->SetDirtyTrueAtThisFrame();
	this->bmIsProjectionMatrixDirty = true;
	this->bmIsViewMatrixDirty = true;
	this->bmIsViewProjectionMatrixDirty = true;
}

void Camera::SetViewportRectX(float value)
{
	this->mViewportRectX = value;
	this->SetDirtyTrueAtThisFrame();
	this->bmIsProjectionMatrixDirty = true;
	this->bmIsViewMatrixDirty = true;
	this->bmIsViewProjectionMatrixDirty = true;
}

void Camera::SetViewportRectY(float value)
{
	this->mViewportRectY = value;
	this->SetDirtyTrueAtThisFrame();
	this->bmIsProjectionMatrixDirty = true;
	this->bmIsViewMatrixDirty = true;
	this->bmIsViewProjectionMatrixDirty = true;
}

void Camera::SetViewportRectWidth(float value)
{
	this->mViewportRectWidth = value;
	this->SetDirtyTrueAtThisFrame();
	this->bmIsProjectionMatrixDirty = true;
	this->bmIsViewMatrixDirty = true;
	this->bmIsViewProjectionMatrixDirty = true;
}

void Camera::SetViewportRectHeight(float value)
{
	this->mViewportRectHeight = value;
	this->SetDirtyTrueAtThisFrame();
	this->bmIsProjectionMatrixDirty = true;
	this->bmIsViewMatrixDirty = true;
	this->bmIsViewProjectionMatrixDirty = true;
}

doom::Camera::eProjectionType Camera::GetProjectionMode() const
{
	return this->mProjectionMode;
}

float Camera::GetFieldOfView() const
{
	return this->mFieldOfViewInDegree;
}

float Camera::GetClippingPlaneNear() const
{
	return this->mClippingPlaneNear;
}

float Camera::GetClippingPlaneFar() const
{
	return this->mClippingPlaneFar;
}

float Camera::GetViewportRectX() const
{
	return this->mViewportRectX;
}

float Camera::GetViewportRectY() const
{
	return this->mViewportRectY;
}

float Camera::GetViewportRectWidth() const
{
	return this->mViewportRectWidth;
}

float Camera::GetViewportRectHeight() const
{
	return this->mViewportRectHeight;
}






const std::array<math::Vector4, 6>& doom::Camera::CalculateFrustumPlane()
{
	auto& viewProjectionMatrix = this->GetViewProjectionMatrix();
	// Left Frustum Plane
   // Add first column of the matrix to the fourth column
	mFrustumPlane[0].x = viewProjectionMatrix[0][3] + viewProjectionMatrix[0][0];
	mFrustumPlane[0].y = viewProjectionMatrix[1][3] + viewProjectionMatrix[1][0];
	mFrustumPlane[0].z = viewProjectionMatrix[2][3] + viewProjectionMatrix[2][0];
	mFrustumPlane[0].w = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][0];
	mFrustumPlane[0].Normalize();

	// Right Frustum Plane
	// Subtract first column of matrix from the fourth column
	mFrustumPlane[1].x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][0];
	mFrustumPlane[1].y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][0];
	mFrustumPlane[1].z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][0];
	mFrustumPlane[1].w = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][0];
	mFrustumPlane[1].Normalize();

	// Top Frustum Plane
	// Subtract second column of matrix from the fourth column
	mFrustumPlane[2].x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][1];
	mFrustumPlane[2].y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][1];
	mFrustumPlane[2].z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][1];
	mFrustumPlane[2].w = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][1];
	mFrustumPlane[2].Normalize();

	// Bottom Frustum Plane
	// Add second column of the matrix to the fourth column
	mFrustumPlane[3].x = viewProjectionMatrix[0][3] + viewProjectionMatrix[0][1];
	mFrustumPlane[3].y = viewProjectionMatrix[1][3] + viewProjectionMatrix[1][1];
	mFrustumPlane[3].z = viewProjectionMatrix[2][3] + viewProjectionMatrix[2][1];
	mFrustumPlane[3].w = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][1];
	mFrustumPlane[3].Normalize();

	// Near Frustum Plane
	// We could add the third column to the fourth column to get the near plane,
	// but we don't have to do this because the third column IS the near plane
	mFrustumPlane[4].x = viewProjectionMatrix[0][2];
	mFrustumPlane[4].y = viewProjectionMatrix[1][2];
	mFrustumPlane[4].z = viewProjectionMatrix[2][2];
	mFrustumPlane[4].w = viewProjectionMatrix[3][2];
	mFrustumPlane[4].Normalize();

	// Far Frustum Plane
	// Subtract third column of matrix from the fourth column
	mFrustumPlane[5].x = viewProjectionMatrix[0][3] - viewProjectionMatrix[0][2];
	mFrustumPlane[5].y = viewProjectionMatrix[1][3] - viewProjectionMatrix[1][2];
	mFrustumPlane[5].z = viewProjectionMatrix[2][3] - viewProjectionMatrix[2][2];
	mFrustumPlane[5].w = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][2];
	mFrustumPlane[5].Normalize();

	return mFrustumPlane;
}

void Camera::InitComponent()
{
	auto currentWorld = Scene::GetSingleton();
	Camera* currentMainCamera = currentWorld->GetMainCamera();
	if (currentMainCamera == nullptr)
	{
		currentWorld->SetMainCamera(this);
	}
}

void Camera::UpdateComponent()
{
	if (this->GetTransform()->GetIsDirtyAtPreviousFrame())
	{
		this->bmIsViewMatrixDirty = true;
		this->bmIsViewProjectionMatrixDirty = true;
	}
}

void Camera::OnEndOfFrame_Component()
{

}

const math::Matrix4x4& doom::Camera::GetProjectionMatrix()
{
	if (this->bmIsProjectionMatrixDirty == true)
	{
		if (this->mProjectionMode == eProjectionType::Perspective)
		{
			this->mProjectionMatrix = math::perspectiveFov(math::DEGREE_TO_RADIAN * this->mFieldOfViewInDegree, static_cast<float>(doom::graphics::Graphics_Server::GetScreenWidth()), static_cast<float>(doom::graphics::Graphics_Server::GetScreenHeight()), this->mClippingPlaneNear, this->mClippingPlaneFar);
		}
		else
		{
			this->mProjectionMatrix = math::ortho(this->mViewportRectX, this->mViewportRectX + this->mViewportRectWidth, this->mViewportRectY, this->mViewportRectY + this->mViewportRectHeight, this->mClippingPlaneNear, this->mViewportRectHeight);
		}

		this->bmIsProjectionMatrixDirty = false;
	}

	return this->mProjectionMatrix;
}

const math::Matrix4x4& Camera::GetViewMatrix()
{
	if (this->bmIsViewMatrixDirty == true)
	{
		auto transform = this->GetTransform();
		auto pos = transform->GetPosition();
		this->mViewMatrix = math::lookAt(pos, pos + transform->forward(), transform->up());
		this->bmIsViewMatrixDirty = false;
	}

	return this->mViewMatrix;
}

const math::Matrix4x4& doom::Camera::GetViewProjectionMatrix()
{
	if (this->bmIsViewProjectionMatrixDirty == true)
	{
		auto& viewMatrix = this->GetViewMatrix();
		auto& projectionMatrix = this->GetProjectionMatrix();
		this->mViewProjectionMatrix = projectionMatrix * viewMatrix;
	}
	return this->mViewProjectionMatrix;
}

math::Vector3 doom::Camera::ScreenToViewportPoint(const math::Vector3& position)
{
	math::Vector3 viewportPoint = position * 2; 
	viewportPoint.x /= graphics::Graphics_Server::GetScreenSize_const().x;
	viewportPoint.y /= graphics::Graphics_Server::GetScreenSize_const().y;
	viewportPoint -= 1;
	viewportPoint.z = 1 / (this->mFieldOfViewInDegree * math::DEGREE_TO_RADIAN / 2);

	return viewportPoint;
}

math::Vector3 doom::Camera::ViewportToScreenPoint(const math::Vector3& position)
{
	math::Vector3 screenPoint = position + 1.0f;
	screenPoint.x *= graphics::Graphics_Server::GetScreenSize_const().x;
	screenPoint.y *= graphics::Graphics_Server::GetScreenSize_const().y;
	screenPoint /= 2;
	screenPoint.z = 0;


	return screenPoint;
}

void Camera::UpdateUniformBufferObjectTempBuffer(graphics::UniformBufferObjectManager& uboManager)
{
	if (Scene::GetSingleton()->GetMainCamera() == this)
	{//if this camera is mainCamera
		auto& projectionMatrix = this->GetProjectionMatrix();

		//!!!! Opengl Use column major of matrix data layout
		uboManager.StoreDataAtTempBufferOfBindingPoint(GLOBAL_UNIFORM_BLOCK_BINDING_POINT, (void*)projectionMatrix.data(), sizeof(projectionMatrix), graphics::eUniformBlock_Global::projection);

		auto transform = this->GetTransform();
		if (transform->GetIsDirtyAtPreviousFrame() == true)
		{//when transform value is changed
			auto& viewMatrix = this->GetViewMatrix(); 
			const auto& camPos = transform->GetPosition();

			uboManager.StoreDataAtTempBufferOfBindingPoint(GLOBAL_UNIFORM_BLOCK_BINDING_POINT, (void*)viewMatrix.data(), sizeof(viewMatrix), graphics::eUniformBlock_Global::view);
			uboManager.StoreDataAtTempBufferOfBindingPoint(GLOBAL_UNIFORM_BLOCK_BINDING_POINT, (void*)camPos.data(), sizeof(camPos), graphics::eUniformBlock_Global::camPos);

		}
	}
}
