#include "TEST.h"

#include <Doom_Core.h>
#include "../Game/AssetManager/AssetManager.h"
#include "../Graphics/Material.h"
#include <Rendering/Renderer/MeshRenderer.h>
#include <Rendering/Camera.h>
#include "UserInput_Server.h"
#include "Move_WASD.h"
#include <Rendering/Light/DirectionalLight.h>
#include "AutoRotate.h"
#include <TestComponent.h>

void doom::TEST::Init()
{
	auto currenScene = doom::Scene::GetCurrentWorld();

	auto lightEntity = currenScene->CreateNewEntity();
	auto dirLight = lightEntity->AddComponent<DirectionalLight>();
	dirLight->SetIntensity(5);
	lightEntity->AddComponent<AutoRotate>();

	//TODO : Asset 가져오는 것만 해도 존나 복잡하다 이거 해결하다
	//auto& threedasset = assetimporter::AssetManager::GetAsset<asset::eAssetType::THREE_D_MODEL>(0);
	auto threedasset = assetimporter::AssetManager::GetAsset<asset::eAssetType::THREE_D_MODEL>("cerberus.assbin");
	auto shader = assetimporter::AssetManager::GetAsset<asset::eAssetType::SHADER>("GbufferWriter_PBR.glsl");
	auto material = new graphics::Material(shader);
	material->AddTexture(graphics::eTextureBindingPoint::AlbedoTexture, assetimporter::AssetManager::GetAsset<asset::eAssetType::TEXTURE>("cerberus_A.dds"));
	material->AddTexture(graphics::eTextureBindingPoint::NormalTexture, assetimporter::AssetManager::GetAsset<asset::eAssetType::TEXTURE>("cerberus_N.dds"));
	material->AddTexture(graphics::eTextureBindingPoint::MetalnessTexture, assetimporter::AssetManager::GetAsset<asset::eAssetType::TEXTURE>("cerberus_M.dds"));
	material->AddTexture(graphics::eTextureBindingPoint::RoughnessTexture, assetimporter::AssetManager::GetAsset<asset::eAssetType::TEXTURE>("cerberus_R.dds"));

	for (int i = 0; i < 30; i++)
	{
		for (int i = 0; i < threedasset->GetMeshCount(); i++)
		{
			auto entity = currenScene->CreateNewEntity();
			entity->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
			entity->GetTransform()->SetPosition(Random::RandomFloatNumber(-10, 10), Random::RandomFloatNumber(-10, 10), Random::RandomFloatNumber(-10, 10));
			auto meshRenderer = entity->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(threedasset->GetMesh(i));
			meshRenderer->SetMaterial(material);
			entity->AddComponent<AutoRotate>();
		}
	}
	


	auto entity1 = currenScene->CreateNewEntity();
	auto entity1Camera = entity1->AddComponent<Camera>();
	entity1Camera->SetProjectionMode(doom::Camera::eProjectionType::Perspective);

	entity1->GetTransform()->SetPosition(0.0f, 0.0f, 1.0f);
	entity1->AddComponent<Move_WASD>();


	auto physicsEntity = currenScene->CreateNewEntity();
	entity1->AddComponent<TestComponent>();
}