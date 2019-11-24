#include "destructible_entity.h"

namespace game {

	DestructibleEntity::DestructibleEntity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : Entity(name, geometry, material, texture, envmap)
	{
		m_hp = 50.0f;
	}


	DestructibleEntity::~DestructibleEntity() 
	{
	}

} // namespace game
