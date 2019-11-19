#pragma once
#include "static_enemy.h"
//Basic Ground Enemy Type
namespace game
{
	class GroundEnemy :
		public StaticEnemy
	{
	public:
		GroundEnemy(const std::string name, const Resource *geometry, const Resource *material);
		~GroundEnemy();

		// Update geometry configuration
		void Init(void);
		void Update(float deltaTime);
		void Idle(float deltaTime);
		void Patrol(float deltaTime);
		void Chase(float deltaTime);
		void Attack(float deltaTime);
		void Die(float deltaTime);

	protected:
		// Angular momentum of asteroid
		glm::quat angm_;
		float chase_radius_;
		float chase_angle_;
		float attack_angle_;

		float acc_;	// Acceleration Rate
		float dec_; // Deceleration Rate

		float vel_;	// Rate of position change

		//Need to handle acceleration, store velocity
	};
}