#ifndef DESTRUCTIBLE_ENTITY_H_
#define DESTRUCTIBLE_ENTITY_H_

#include "entity.h"

namespace game {

	// Abstraction of an asteroid
	class DestructibleEntity : public Entity {

	public:
		// Create asteroid from given resources
		DestructibleEntity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);

		// Destructor
		~DestructibleEntity();

	private:
		float m_hp;
	}; // class Asteroid

} // namespace game

#endif // DESTRUCTIBLE_ENTITY_H_
