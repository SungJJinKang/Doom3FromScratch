#pragma once
#include "../Core.h"
#include "GameFlow.h"
#include "../../Helper/Singleton.h"
#include "../../Helper/SimpleIniParser.h"

#include <memory>


#include "AssetManager.h"

#include "../Scene/SharedScene.h"
#include "../Scene/Scene.h"

#include "../Graphics/Graphics_Server.h"
#include "../Physics/Physics_Server.h"
#include "../ResourceManagement/Thread_Server.h"

namespace doom
{

	class GameFlow;

	class GameCore : public GameFlow, public ISingleton<GameCore>
	{
		friend class graphics::Graphics_Server;
		friend class AssetManager;
	private:
		const char* mConfigFilePath{};
		IniData mConfigData{};

		//Servers
		assetimporter::AssetManager mAssetManager{};
		graphics::Graphics_Server mGraphics_Server{};
		physics::Physics_Server mPhysics_Server{};
		resource::Thread_Server mThreadManager{};

		std::unique_ptr<Scene> mCurrentScene{};
		std::unique_ptr<Scene> CreateNewScene(std::string sceneName = "");

		SharedScene mSharedWorld{};

		
	public:
		

		GameCore();
		GameCore(const GameCore&) = delete;
		GameCore(GameCore&&) = delete;
		GameCore& operator=(const GameCore&) = delete;
		GameCore& operator=(GameCore&&) = delete;

		IniData& GetConfigData();


		virtual void Init() final;
		/// <summary>
		/// Frame Loop
		/// </summary>
		virtual void Update() final
		{
			D_START_PROFILING("Update Physics", eProfileLayers::CPU);
			this->mPhysics_Server.Update();
			D_END_PROFILING("Update Physics");

			D_START_PROFILING("Update PlainComponents", eProfileLayers::CPU);
			this->mCurrentScene->UpdatePlainComponents();
			D_END_PROFILING("Update PlainComponents");

			D_START_PROFILING("GraphicsUpdate", eProfileLayers::GPU);
			this->mGraphics_Server.Update();
			D_END_PROFILING("GraphicsUpdate");

			this->OnEndOfFrame();
		}

		virtual void OnEndOfFrame() final
		{
			this->mPhysics_Server.OnEndOfFrame();

			this->mCurrentScene->OnEndOfFrameOfEntities();

			this->mGraphics_Server.OnEndOfFrame();

		
		}

	};
}
