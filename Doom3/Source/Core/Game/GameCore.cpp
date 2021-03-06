#include "GameCore.h"

#include "../Graphics/GraphicsAPI.h"
#include "../Graphics/Graphics_Server.h"

#include "../../Helper/SimpleIniParser.h"
#include "AssetManager/AssetManager.h"

#include "../../GameLogic/TEST.h"
#include "ConfigData.h"

#include "AssetImporter/AssetImporterWorker_Texture.h"

#include "../Logger/Logger.h"
#include "../Profiler/Profiler.h"


doom::GameCore::GameCore()
{
	this->mCurrentScene = this->CreateNewScene();
}




std::unique_ptr<doom::Scene> doom::GameCore::CreateNewScene(std::string sceneName /*= ""*/)
{
	return std::unique_ptr<doom::Scene>{new Scene(sceneName)};
}

void doom::GameCore::InitGameSetting()
{
	this->ITERATION_PER_SECOND = this->mMainConfigData.GetConfigData().GetValue<int>("SYSTEM", "ITERATION_PER_SECOND");
	this->TARGET_FRAME_RATE = this->mMainConfigData.GetConfigData().GetValue<int>("SYSTEM", "TARGET_FRAME_RATE");
	this->FRAME_DELAY_MILLISECOND = this->mMainConfigData.GetConfigData().GetValue<int>("SYSTEM", "FRAME_DELAY_MILLISECOND");

	this->mPhysics_Server.bmIsPhysicsOn = static_cast<bool>(this->mMainConfigData.GetConfigData().GetValue<int>("PHYSICS", "ENABLE"));
	this->mPhysics_Server.FIXED_TIME_STEP = static_cast<float>(this->mMainConfigData.GetConfigData().GetValue<double>("PHYSICS", "FIXED_TIME_STEP"));
	this->mPhysics_Server.MAX_PHYSICS_STEP = this->mMainConfigData.GetConfigData().GetValue<int>("PHYSICS", "MAX_PHYSICS_STEP");
	
	this->mPhysics_Server.ENLARGED_AABB2D_OFFSET = static_cast<float>(this->mMainConfigData.GetConfigData().GetValue<double>("PHYSICS", "ENLARGED_AABB2D_OFFSET"));
	this->mPhysics_Server.ENLARGED_AABB3D_OFFSET = static_cast<float>(this->mMainConfigData.GetConfigData().GetValue<double>("PHYSICS", "ENLARGED_AABB3D_OFFSET"));
	
	assetimporter::AssetImporterWorker<::doom::asset::eAssetType::TEXTURE>::TEXTURE_COMPRESSION_QUALITY = static_cast<float>(this->mMainConfigData.GetConfigData().GetValue<double>("TEXTURE", "COMPRESSION_QUALITY"));
	assetimporter::AssetImporterWorker<::doom::asset::eAssetType::TEXTURE>::MIP_MAP_LEVELS = this->mMainConfigData.GetConfigData().GetValue<int>("TEXTURE", "MIP_MAP_LEVELS");
	assetimporter::AssetImporterWorker<::doom::asset::eAssetType::TEXTURE>::MAX_IMAGE_SIZE = this->mMainConfigData.GetConfigData().GetValue<int>("TEXTURE", "MAX_IMAGE_SIZE");

#ifdef DEBUG_MODE
	doom::logger::Logger::InitLogger();
	doom::profiler::Profiler::InitProfiling();
#endif

}

void doom::GameCore::UpdateGameCore()
{
#ifdef DEBUG_MODE
	if (userinput::UserInput_Server::GetKeyUp(eKEY_CODE::KEY_0))
	{
		doom::profiler::Profiler::SetActiveToggle();
	}
#endif

}

void doom::GameCore::Init()
{
	D_START_PROFILING("InitGameSetting", eProfileLayers::CPU);
	this->InitGameSetting();
	D_END_PROFILING("InitGameSetting");

	D_START_PROFILING("mTime_Server Init", eProfileLayers::CPU);
	this->mTime_Server.Init();
	D_END_PROFILING("mTime_Server Init");

	//
	//Read This : https://docs.unity3d.com/Manual/class-TimeManager.html
	D_START_PROFILING("Init Physics_Server", eProfileLayers::CPU);
	this->mPhysics_Server.Init();
	D_END_PROFILING("Init Physics_Server");

	D_START_PROFILING("Init Graphics_Server", eProfileLayers::Rendering);
	this->mGraphics_Server.Init();
	D_END_PROFILING("Init Graphics_Server");

	D_START_PROFILING("Init UserInput_Server", eProfileLayers::CPU);
	this->mUserImput_Server.Init();
	D_END_PROFILING("Init UserInput_Server");



	D_START_PROFILING("ImportEntireAsset", doom::profiler::eProfileLayers::CPU);
	this->mAssetManager.ImportEntireAsset();
	D_END_PROFILING("ImportEntireAsset");



	this->LateInit();
}

void doom::GameCore::LateInit()
{
	/*
	D_START_PROFILING("Init Physics_Server", eProfileLayers::CPU);
	this->mPhysics_Server.Init();
	D_END_PROFILING("Init Physics_Server");
	*/

	D_START_PROFILING("LateInit Graphics_Server", eProfileLayers::Rendering);
	this->mGraphics_Server.LateInit();
	D_END_PROFILING("LateInit Graphics_Server");

	/*
	D_START_PROFILING("Init UserInput_Server", eProfileLayers::CPU);
	this->mUserImput_Server.Init();
	D_END_PROFILING("Init UserInput_Server");
	*/
	TEST::Init();
}

void doom::GameCore::CleanUp()
{

}
