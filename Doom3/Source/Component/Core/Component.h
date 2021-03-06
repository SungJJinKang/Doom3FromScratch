#pragma once

#include "../../Core/Core.h"
#include "../../Core/Game/IGameFlow.h"
#include "../../Core/Game/FrameDirtyChecker.h"

class DirtyReceiver;
namespace doom
{
	class Entity;
	class Transform;
	class Scene;
	
	/// <summary>
	/// Component Of Entity
	/// 
	/// HOW TO CREATE MY OWN COMPONENT :
	/// Don't inherit Component class
	/// Inherit ServerComponent Or PlainComponent
	/// 
	/// Never make Constructor with argument, Component Constructor should not have argument
	/// 
	/// Both ones have different 
	/// </summary>
	class Component : public FrameDirtyChecker // : public GameFlow
	{
		friend class Entity;
		friend class Scene;
		friend class vector;

		struct Deleter
		{
			void operator()(Component* component) const
			{
				delete component;
			}
		};
	private:

		//For Prevent Component copied or moved
		Component(const Component&) = delete;
		Component(Component&&) noexcept = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) noexcept = delete;

		bool bIsAddedToEntity;
		Entity* mOwnerEntity;
		/// <summary>
		/// Cache
		/// </summary>
		Transform* mTransform;
		
		bool mIsActivated;

	
	protected:

		void AddLocalDirtyToTransformDirtyReceiver(DirtyReceiver& localDirty);

		Component();

		/// <summary>
		/// Pure virtual destructor for make this class virtual cass
		/// Destructor should be called only from RemoveConponent(or clear component) of Entity class
		/// </summary>
		/// <returns></returns>
		virtual ~Component(){}

	
		/// Dont put public to Internal Function For Protect use call Internal Function
		virtual void InitComponent_Internal(Entity* entity);
		virtual void InitComponent()
		{
			//DONT PUT ANYTHING HERE, PUT AT HERE
		}
		virtual void UpdateComponent_Internal();
		virtual void UpdateComponent()
		{
			//DONT PUT ANYTHING HERE, PUT AT HERE
		}

		virtual void FixedUpdateComponent_Internal();
		virtual void FixedUpdateComponent()
		{
			//DONT PUT ANYTHING HERE, PUT AT HERE
		}

		virtual void OnEndOfFrame_Component_Internal();
		virtual void OnEndOfFrame_Component()
		{
			//DONT PUT ANYTHING HERE, PUT AT HERE
		}

		/// <summary>
		/// This function will be called before Component object is destroyed
		/// </summary>
		virtual void OnDestroy_Internal();
		/// <summary>
		/// This function will be called before Component object is destroyed
		/// </summary>
		virtual void OnDestroy()
		{
			//DONT PUT ANYTHING HERE, PUT AT HERE
		}


		/// <summary>
		/// This function will be called after activated
		/// </summary>
		virtual void OnActivated_Internal();
		/// <summary>
		/// This function will be called after activated
		/// </summary>
		virtual void OnActivated()
		{
			//DONT PUT ANYTHING HERE, PUT AT HERE
		}


		/// <summary>
		/// This function will be called after deactivated
		/// </summary>
		virtual void OnDeActivated_Internal();
		/// <summary>
		/// This function will be called after deactivated
		/// </summary>
		virtual void OnDeActivated()
		{
			//DONT PUT ANYTHING HERE, PUT AT HERE
		}


		/* can't do this because of recursive dependent
		template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
		[[nodiscard]] constexpr T* GetComponent() // never return unique_ptr reference, just return pointer
		{
			return this->GetOwnerEntity()->GetComponent<T>();
		}

		template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
		[[nodiscard]] constexpr std::vector<T*> GetComponents()
		{
			return this->GetOwnerEntity()->GetComponents<T>();
		}
		*/

// 		virtual void OnComponentAttachedToOwnerEntity(Component* newComponent) {}
// 		virtual void OnComponentDettachedToOwnerEntity(Component* newComponent) {}

	public:

	
		FORCE_INLINE constexpr Entity* GetOwnerEntity() const
		{
			D_ASSERT(this->mOwnerEntity); // mOwnerEntity is set at InitComponent_Internal ( not Constructor )
			return this->mOwnerEntity;
		}
		unsigned int GetOwnerEntityLayerIndex() const;
		FORCE_INLINE constexpr Transform* GetTransform()
		{
			return this->mTransform;
		}
		FORCE_INLINE constexpr const Transform* GetTransform() const
		{
			return this->mTransform;
		}
		

		
		
		// TODO : Add Preupdate
		//virtual void OnPreUpdateComponent() {}
		//virtual void OnPostUpdateComponent() {}
	};
}