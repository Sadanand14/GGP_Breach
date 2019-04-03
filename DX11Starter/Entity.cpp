#include "Entity.h"

using namespace DirectX;

#include "Scene.h"

void Entity::DestroyInternal()
{
	u64 cc = children.size();
	for (u64 i = 0; i < cc; ++i)
	{
		((Entity*)(children[i]))->Destroy();
	}

	if (parent != nullptr) { parent->RemoveChild(this); }

	// Safety block - maybe remove later for performance?
	children.clear();
	parent = nullptr;
	scene = SceneRef();

	// TODO : Perform component login here
}

Entity::Entity() :
	Object(),
	tags(),
	scene()
{
	// Nothing interesting to do here
}

Entity::Entity(Scene* parentScene, u64 ind, u64 sID, u64 tID, char* fileName, ID3D11Device* device, Material* mat, const Transform& t, Entity* parentEntity) :
	Object(t, parentEntity),
	tags(),
	scene(parentScene, ind, sID, tID),
	meshObject(nullptr),
	material(nullptr)
{
	meshObject = new Mesh(fileName, device);
	material = mat;
}

void Entity::Destroy()
{
	scene.GetScene()->DestroyEntity(this);
}

void Entity::AddTag(std::string tag)
{
	// TODO : Add logic for interfacing scene - various tag optimizations
	tags.push_back(tag);
}

void Entity::RemoveTag(std::string tag)
{
	// TODO : Add logic for interfacing scene - various tag optimizations
	for (u64 i = 0; i < tags.size(); ++i)
	{
		if (tags[i] == tag)
		{
			tags[i] = tags[tags.size() - 1];
			tags.pop_back();
			return;
		}
	}
}

void Entity::AddTags(std::vector<std::string> newTags)
{
	// TODO : Add logic for interfacing scene - various tag optimizations
	for (u64 i = 0; i < newTags.size(); ++i)
	{
		tags.push_back(newTags[i]);
	}
}

Entity::~Entity()
{
	// delete meshObject;
}

void Entity::PrepareShader(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix)
{
	material->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	material->GetVertexShader()->SetMatrix4x4("projection", projMatrix);
	mat4 worldMat = glm::transpose(GetWorldMatrix());
	float* matarr = &(worldMat[0][0]);
	material->GetVertexShader()->SetMatrix4x4("world", matarr);

	material->GetPixelShader()->SetShaderResourceView("res", material->GetShaderResourceView());
	material->GetPixelShader()->SetShaderResourceView("normalMap", material->GetNormalResourceView());
	material->GetPixelShader()->SetSamplerState("state", material->GetSamplerState());

	material->GetVertexShader()->CopyAllBufferData();
	material->GetPixelShader()->CopyAllBufferData();

	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();
}