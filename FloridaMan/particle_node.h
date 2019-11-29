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
			ParticleNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
			ParticleNode(const std::string name);
			ParticleNode();

            // Destructor
            ~ParticleNode();

			void Update(float deltaTime);

			//Setup Shader for Particle Shader
			void SetupShader(GLuint program, Camera *camera);
			void Start(void);
			float GetStart(void);

        private:
			float start;
			float duration;
			bool looping;
    }; // class Asteroid

} // namespace game

#endif // PARTICLE_NODE