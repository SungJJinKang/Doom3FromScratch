#pragma once
#include "Core/PlainComponent.h"

#include "../Core/Math/LightMath_Cpp/Vector4.h"
#include "../Core/Math/LightMath_Cpp/Matrix4x4.h"

#include <array>
#include "../Core/Graphics/Buffer/UniformBufferObjectTempBufferUpdater.h"
#include <StaticContainer/StaticContainer.h>

#include <EasyDirtyChecker/DirtyReceiver.h>

namespace doom
{
	class Camera : public PlainComponent, public graphics::UniformBufferObjectTempBufferUpdater, public StaticContainer<Camera>
	{
		friend class Scene;
	public:
		enum class eProjectionType
		{
			Perspective,
			Orthographic
		};
	private:

	
		Camera(const Camera&) = delete;
		Camera(Camera&&) noexcept = delete;
		Camera& operator=(const Camera&) = delete;
		Camera& operator=(Camera&&) noexcept = delete;

		eProjectionType mProjectionMode{ eProjectionType::Perspective };

		///
		float mFieldOfViewInDegree = 60;

		float mClippingPlaneNear = 0.1f;
		float mClippingPlaneFar = 1000.0f;

		/// <summary>
		/// opengl -1 ~ 1
		/// </summary>
		float mViewportRectX = -1.0f;
		/// <summary>
		/// opengl -1 ~ 1
		/// </summary>
		float mViewportRectY = -1.0f;

		/// <summary>
		/// opengl -1 ~ 1
		/// </summary>
		float mViewportRectWidth = 2.0f;
		/// <summary>
		/// opengl -1 ~ 1
		/// </summary>
		float mViewportRectHeight = 2.0f;

		DirtyReceiver bmIsProjectionMatrixDirty{ true };
		DirtyReceiver bmIsViewMatrixDirty{ true };
		DirtyReceiver bmIsViewProjectionMatrixDirty{ true };
		DirtyReceiver bmIsFrustumPlaneMatrixDirty{ true };
		math::Matrix4x4 mViewMatrix{};
		math::Matrix4x4 mProjectionMatrix{};
		math::Matrix4x4 mViewProjectionMatrix{};
		std::array<math::Vector4, 6> mFrustumPlane{};
		const std::array<math::Vector4, 6>& CalculateFrustumPlane();

		virtual void InitComponent() final;
		virtual void UpdateComponent() final;
		virtual void OnEndOfFrame_Component() final;

		DirtyReceiver bmIsUboDirty{ true };
		void UpdateUniformBufferObjectTempBuffer(graphics::UniformBufferObjectManager& uboManager) final;

		void UpdateMainCamera();
		/// <summary>
		/// This object Component is set to Scene's MainCamera
		/// </summary>
		void OnSetMainCamera();
	public:

		Camera() = default;
		virtual ~Camera() = default;

		void SetProjectionMode(eProjectionType value);
		void SetFieldOfView(float value);
		void SetClippingPlaneNear(float value);
		void SetClippingPlaneFar(float value);
		void SetViewportRectX(float value);
		void SetViewportRectY(float value);
		void SetViewportRectWidth(float value);
		void SetViewportRectHeight(float value);

		eProjectionType GetProjectionMode() const;
		float GetFieldOfView() const;
		float GetClippingPlaneNear() const;
		float GetClippingPlaneFar() const;
		float GetViewportRectX() const;
		float GetViewportRectY() const;
		float GetViewportRectWidth() const;
		float GetViewportRectHeight() const;
	
		static Camera* GetMainCamera();

		/// <summary>
		/// this function will be called at every frame
		/// </summary>
		/// <returns></returns>
		const math::Matrix4x4& GetProjectionMatrix();
		/// <summary>
		/// this function will be called at every frame
		/// </summary>
		/// <returns></returns>
		const math::Matrix4x4& GetViewMatrix();
		const math::Matrix4x4& GetViewProjectionMatrix();

		[[nodiscard]] math::Vector3 NDCToScreenPoint(const math::Vector3& ndcPoint);
		[[nodiscard]] math::Vector3 ScreenToNDCPoint(const math::Vector3& screenPoint);
		
		[[nodiscard]] math::Vector3 WorldToNDCPoint(const math::Vector3& worldPosition);
		[[nodiscard]] math::Vector3 NDCToWorldPoint(const math::Vector3& ndcPoint);

		[[nodiscard]] math::Vector3 WorldToScreenPoint(const math::Vector3& worldPosition);
		[[nodiscard]] math::Vector3 ScreenToWorldPoint(const math::Vector3& screenPosition);

		// we don't use ViewPort
		// We use NDC. x : -1 ~ 1, y : -1 ~ 1, z : depth value * 2 - 1, relative to bottom-left corner

		/// <summary>
		/// Top of screen has negative y value
		/// 
		/// Screen coordinate is relative to top-left corner of screen
		/// </summary>
		/// <param name="screenPosition"></param>
		/// <returns></returns>
		[[deprecated]] math::Vector3 ScreenToViewportPoint(const math::Vector3& screenPosition);
		/// <summary>
		/// Top of screen has negative y value
		/// 
		/// Screen coordinate is relative to top-left corner of screen
		/// </summary>
		/// <param name="viewportPosition"></param>
		/// <returns></returns>
		[[deprecated]] math::Vector3 ViewportToScreenPoint(const math::Vector3& viewportPosition);

		
protected:
	void OnDestroy() override;

	};
}