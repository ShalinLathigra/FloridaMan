#ifndef MINE_H_
#define MINE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"
#include "enemy.h"

#include <algorithm>    // std::max

namespace game
{
	enum MineState {MineIdle, MineChase, Boom};

	class Mine :
		public SceneNode
	{
	public:
		Mine(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
		~Mine();

		virtual void Init(void);

		void Update(float deltaTime);

		virtual void Idle(float deltaTime);
		virtual void Chase(float deltaTime);
		virtual void Boom(float deltaTime);


		MineState GetState(void);
		Enemy* GetTarget(void);
		void SetState(MineState state);
		void SetTarget(Enemy* target);

	protected:
		MineState state_;
		Enemy* target_;
		glm::vec3 forward_;
		glm::vec3 up_;
		float hp_;

		float idle_timer_;
		float max_idle_timer_;
	};
}

#endif // MINE_H_
