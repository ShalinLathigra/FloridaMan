#ifndef ENEMY_H_
#define ENEMY_H_

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
	enum State { Patrol, Chase, Attack, Die };

    // Abstraction of an asteroid
    class Enemy : public SceneNode {

        public:
            // Create asteroid from given resources
			Enemy(const std::string name, const Resource *geometry, const Resource *material);

            // Destructor
            ~Enemy();


            // Update geometry configuration
            void Update(float deltaTime);
			
			
			virtual void Patrol(float deltaTime);// Patrol State : Patrol -> Chase, Die
			virtual void Chase(float deltaTime);// Chase State : Chase -> Attack
			virtual void Attack(float deltaTime);// Attack State : Attack -> Chase, Patrol, Die
			virtual void Die(float deltaTime);// Die State : Die -> Die


			//Getters
			glm::quat GetPatrolAng(void);

			//Setters
			void SetTarget(const glm::vec3 & target);
			void SetPatrolAng(glm::quat ang);

        private:
			glm::vec3 & target_;	//Store const reference to target's position.
			float hp_;	//Current HP
			State current_state_;	//Current State

			glm::quat patrol_ang_;	//Enemy Patrol Angle
    }; // class Enemy

} // namespace game

#endif // ENEMY_H_
