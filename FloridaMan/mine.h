#ifndef MINE_H_
#define MINE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <algorithm>    // std::max

#include "resource.h"
#include "scene_node.h"

namespace game
{
	enum MineState {MineIdle, MineChase, Boom};

	class Mine :
		public SceneNode
	{
	public:
		Mine(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
		~Mine();

		void Update(float deltaTime);

		void Idle(float deltaTime);
		void Chase(float deltaTime);
		void Boom(float deltaTime);

		MineState GetState(void);
		SceneNode* GetTarget(void);
		void SetState(MineState state);
		void SetTarget(SceneNode* target);

	protected:
		MineState state_;
		SceneNode* target_;
		glm::vec3 forward_; 
		glm::vec3 up_;
		float hp_;
		bool target_set_;
		float accel_ = 4.0f;
		std::vector<SceneNode> enemies;

		float idle_timer_;
		float max_idle_timer_;
	};
}

#endif // MINE_H_
