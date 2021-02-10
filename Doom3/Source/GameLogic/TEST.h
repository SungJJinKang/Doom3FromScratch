#pragma once

#include <Entity.h>
#include <Scene.h>
#include <Camera.h>
#include <MeshRenderer.h>
#include "../Game/AssetManager.h"


namespace doom
{
	class TEST
	{
	public:

		static void Init()
		{
			auto currenScene = doom::Scene::GetCurrentWorld();
			auto entity1 = currenScene->CreateNewEntity();
			auto entity1Camera = entity1->AddComponent<Camera>();

			auto entity2 = currenScene->CreateNewEntity();
			auto meshRenderer = entity2->AddComponent<MeshRenderer>();

			//TODO : Asset �������� �͸� �ص� ���� �����ϴ� �̰� �ذ��ϴ�
			meshRenderer->SetMesh(&(assetimporter::AssetManager::GetAsset<eAssetType::THREE_D_MODEL>(0).value().get().GetMesh(0)));
			auto& shader = assetimporter::AssetManager::GetAsset<eAssetType::SHADER>(0).value().get();
			auto material = new graphics::Material(shader);
			meshRenderer->SetMaterial(material);

			//meshRenderer->
		}
	};
}


