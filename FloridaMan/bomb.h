#ifndef BOMB_H_
#define BOMB_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <algorithm>    // std::max

#include "resource.h"
#include "scene_node.h"

namespace game
{
	enum BombState { BombFall, BombBoom };
	
	class Bomb :
		public SceneNode
	{
	public:
		Bomb(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
		~Bomb();


		void Update(float deltaTime);

		void Fall(float deltaTime);
		void Boom(float deltaTime);

		BombState GetState(void);
		void SetState(BombState state);

	protected:
		BombState state_;
		glm::vec3 forward_;
		glm::vec3 up_;
		glm::vec3 velocity_ = glm::vec3(0,0,0);

		float idle_timer_;
		float max_idle_timer_;

		float boom_radius_;
	};
}

#endif // MINE_H_
