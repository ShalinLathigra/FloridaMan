#pragma once

#ifndef ENEMY_H_
#define ENEMY_H_

#include "scene_node.h"
namespace game
{
	enum State { Idle, Patrol, Chase, Attack, Die};

	//Enemy Superclass
	class Enemy :
		public SceneNode
	{
	public:
		Enemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
		~Enemy();
		
		void Update(float deltaTime);

		virtual void Idle(float deltaTime);
		virtual void Patrol(float deltaTime);
		virtual void Chase(float deltaTime);
		virtual void Attack(float deltaTime);
		virtual void Die(float deltaTime);
		virtual void TakeDamage(float amount);


		State GetState(void);
		Camera* GetTarget(void);
		glm::vec3 GetForward(void);
		glm::vec3 GetUp(void);
		void SetState(State state);
		void SetTarget(Camera* target);

	protected:
		State state_;
		Camera* target_;
		glm::vec3 forward_;
		glm::vec3 up_;
		float hp_;

		float idle_timer_;
		float max_idle_timer_;
	};


}
#endif // Enemy_H_