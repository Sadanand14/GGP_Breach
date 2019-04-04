#ifndef SCENE_H_
#define SCENE_H_

#include "Entity.h"

#include <vector>

class Scene
{
private:
	std::vector<Entity> entities;     // All entities belonging to this scene (contains gaps)
	std::vector<Entity*> entitiesTop; // References to top level (parent-less) entities in this scene
	std::vector<Entity*> entitiesAll; // References to all entities in this scene
	
	std::vector<u64> entityArrayGaps; // All known gaps in the entities array

public:
	Scene();
	~Scene();

	// Spawns a new entity into the scene
	Entity* SpawnEntity(Mesh* mesh, Material* mat, Entity* parent = nullptr, const Transform& transform = Transform());

	// Destroys an entity belonging to this scene
	void DestroyEntity(Entity* entity);
};

#endif /* SCENE_H_ */
