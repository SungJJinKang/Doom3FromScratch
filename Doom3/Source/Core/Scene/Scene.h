#pragma once
#include <vector>
#include <memory>
#include "Entity.h"
#include "../Game/IGameFlow.h"
#include "SharedSceneSetting.h"
#include "../Graphics/SceneGraphics.h"

namespace doom
{
	namespace graphics
	{
		class Graphics_Server;
	}

	class Scene;
	class Camera;
	/// <summary>
	/// This class is same with SCENE in unity game engine
	/// </summary>
	class Scene : public ISingleton<Scene>//, public GameFlow
	{

		friend class GameCore;
		friend class graphics::Graphics_Server;

	private:

		size_t mEntityIDCounter{ 0 };
		std::vector<std::unique_ptr<Entity, Entity::Deleter>> mSpawnedEntities{};
		Camera* mMainCamera{ nullptr };

		graphics::SceneGraphics mSceneGraphics{};

		Scene(std::string sceneName = "");

		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;


		
		

	public:

		~Scene();

		static Scene* GetCurrentWorld();

		[[nodiscard]] Entity* CreateNewEntity() noexcept;
		bool DestroyEntity(Entity& entity);

		[[nodiscard]] Camera* GetMainCamera() const;
		void SetMainCamera(Camera* camera);

	protected:
		void FixedUpdatePlainComponents();
		void UpdatePlainComponents();
		void OnEndOfFrameOfEntities();

	public:
	};

}