#ifndef PARTICLE_NODE_H_
#define PARTICLE_NODE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"

namespace game {

    // Abstraction of an asteroid
    class ParticleNode : public SceneNode {

        public:
            // Create asteroid from given resources
			ParticleNode(const std::string name, const Resource *geometry, const Resource *material);

            // Destructor
            ~ParticleNode();
                        
        private:
    }; // class Asteroid

} // namespace game

#endif // PARTICLE_NODE