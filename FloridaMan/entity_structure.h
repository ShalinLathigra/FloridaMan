#ifndef ENTITY_STRUCTURE_H_
#define ENTITY_STRUCTURE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"
#include "air_entity.h"

#define MAX_SPAWN_TIMER 10.0f
namespace game {
    // Abstraction of an EntityStructure
    class EntityStructure : public SceneNode {

        public:
            // Create EntityStructure from given resources
            EntityStructure(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *envmap = NULL);

            // Destructor
            ~EntityStructure();

            // Update geometry configuration
			void Update(float deltaTime);
			void InitResources(int type, Resource* obj, Resource* mat, Resource* tex, Resource* env);

        protected:
			SceneNode* CreateEntity();

			float m_EntityTimer;
			int m_EntityCount;
			int m_MaxEntityCount;

			Resource *m_geom;
			Resource *m_mat;
			Resource *m_tex;
			Resource *m_env;
			int m_type;
			int m_count = 0;
    }; // class EntityStructure

} // namespace game

#endif // ENTITY_STRUCTURE_H_
