#ifndef ENTITY_H_
#define ENTITY_H_

#include "Object.h"
#include "SceneRef.h"

#include "Mesh.h"
#include "Material.h"

#include <vector>
#include <string>

class Scene;
class Component;

class Entity : public Object
{
private:
	std::vector<std::string> tags;
	SceneRef scene;

	// Actually handles internal entity deletion, called by managing scene
	void DestroyInternal();

	friend class Scene;

public:
	Mesh * meshObject = nullptr;
	Material * material = nullptr;

	Entity();
	Entity(Scene* parentScene, u64 ind, u64 sID, u64 tID, Mesh* mesh, Material* mat, const Transform& t = Transform(), Entity* parentEntity = nullptr);
	~Entity();

	inline void SetPositionF(float x, float y, float z) { SetPositionWorld(glm::vec3(x, y, z)); }
	inline void SetScaleF(float x, float y, float z) { SetScale(glm::vec3(x, y, z)); }
	inline void SetRotationF(float x, float y, float z) { SetRotationWorld(glm::quat(glm::vec3(x, y, z))); }
	inline void GetPosition();
	inline void GetScale();
	inline void GetRotation();
	void PrepareShader(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix);

	// Flags the entity for deletion
	void Destroy();

	// <TAGS>

	inline std::vector<std::string> GetTags() const { return tags; }
	inline bool HasTag(std::string tag) const { for (u64 i = 0; i < tags.size(); ++i) { if (tags[i] == tag) { return true; } } return false; }

	// Will probably move tag system purely into script-side once that's up-and-running
	void AddTag(std::string tag);
	void RemoveTag(std::string tag);
	void AddTags(std::vector<std::string> newTags);

	// </TAGS>
};

#endif
