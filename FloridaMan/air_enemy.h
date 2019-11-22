#pragma once
#include "ground_enemy.h"
//Basic Ground Enemy Type
namespace game
{
	class AirEnemy :
		public GroundEnemy
	{
	public:
		AirEnemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
		~AirEnemy();

		// Update geometry configuration
		void Init(void);
		void Update(float deltaTime);
		void Patrol(float deltaTime);
		void Chase(float deltaTime);
		void Attack(float deltaTime);


		void MaintainY(glm::vec3 target_pos, float deltaTime);

	protected:

		float mid_y_;
		int y_offset_[3];
		int off_index_;
		int off_step_;
		float desired_y_;

		float off_timer_;
		float max_off_timer_;

		float y_diff_;
		float y_speed_;

		glm::quat chase_quat_;
		//Need to handle acceleration, store velocity
	};
}