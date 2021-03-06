#include "Entity.h"

#include <algorithm>

#include"Scene.h"
#include <SequenceStringGenerator/SequenceStringGenerator.h>

using namespace doom;

Entity::Entity(size_t entityID, Entity* parent) : mEntityID{ entityID }, mEntityName {}, mPlainComponents{}, mParent{ parent }, mChilds{}
{
	this->mTransform = this->AddComponent<Transform>();
}

Entity::~Entity()
{
	this->ClearComponents();
}


void doom::Entity::InitEntity()
{
}

void doom::Entity::UpdateEntity()
{
}

void doom::Entity::OnEndOfFramePlainComponentsAndEntity()
{
	this->EndOfFrame_PlainComponent();
	this->FrameDirtyChecker_EndOfFrame();
}


void Entity::FixedUpdate_PlainComponent()
{
	for (size_t i = 0; i < this->mPlainComponents.size(); i++)
	{
		this->mPlainComponents[i]->FixedUpdateComponent_Internal();
		this->mPlainComponents[i]->FixedUpdateComponent();
	}
}

void doom::Entity::Update_PlainComponent()
{
	for (size_t i = 0; i < this->mPlainComponents.size(); i++)
	{
		//D_START_PROFILING(SequenceStringGenerator::GetLiteralString("Update PlainComponents ", i), eProfileLayers::CPU);
		this->mPlainComponents[i]->UpdateComponent_Internal();
		this->mPlainComponents[i]->UpdateComponent();
		//D_END_PROFILING(SequenceStringGenerator::GetLiteralString("Update PlainComponents ", i));
	}
}

void doom::Entity::EndOfFrame_PlainComponent()
{
	for (size_t i = 0; i < this->mPlainComponents.size(); i++)
	{
		this->mPlainComponents[i]->OnEndOfFrame_Component_Internal();
		this->mPlainComponents[i]->OnEndOfFrame_Component();
	}
}

void Entity::OnActivated()
{
	this->SetDirtyTrueAtThisFrame();
}

std::string_view Entity::GetEntityName() const
{
	return this->mEntityName;
}


void Entity::AddLayerChangedCallback(void(*callback_ptr)(Entity&))
{
#ifdef DEBUG_MODE
	//check is callback is already contained
	auto iter = std::find(this->mLayerIndexChangedCallback.begin(), this->mLayerIndexChangedCallback.end(), callback_ptr);
	D_ASSERT(iter == this->mLayerIndexChangedCallback.end());
#endif

	this->mLayerIndexChangedCallback.push_back(callback_ptr);
}

void Entity::RemoveLayerChangedCallback(void(*callback_ptr)(Entity&))
{
	auto iter = std::find(this->mLayerIndexChangedCallback.begin(), this->mLayerIndexChangedCallback.end(), callback_ptr);
	D_ASSERT(iter != this->mLayerIndexChangedCallback.end());

	this->mLayerIndexChangedCallback.erase(iter);
}

void Entity::SetLayerIndex(unsigned int layerIndex)
{
	D_ASSERT(layerIndex >= 0 && layerIndex < MAX_LAYER_COUNT);
	this->mLayerIndex = layerIndex;

	for (auto& callback : this->mLayerIndexChangedCallback)
	{
		callback(*this);
	}
}

void Entity::Destroy()
{
	//Work Flow : Scene::GetCurrentWorld().DestroyEntity -> delete Entity -> Entity::~Entity
	Scene::GetCurrentWorld()->DestroyEntity(*this);
	
}

