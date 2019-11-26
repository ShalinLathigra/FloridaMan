#include "particle_node.h"

namespace game {

	ParticleNode::ParticleNode(const std::string name, const Resource *geometry, const Resource *material) : SceneNode(name, geometry, material) 
	{
	}


	ParticleNode::~ParticleNode()
	{
	}
            
} // namespace game
