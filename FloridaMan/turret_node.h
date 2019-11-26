#ifndef TurretNode_H_
#define TurretNode_H_

#include "entity.h"

//Basic Static Entity Type
namespace game
{
	class TurretNode :
		public Entity
	{
	public:
		TurretNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
		~TurretNode();

		// Update geometry configuration
		void Update(float deltaTime);
		void Idle(float deltaTime);
		void Patrol(float deltaTime);
		void Chase(float deltaTime);
		void Attack(float deltaTime);
		void Die(float deltaTime);

		// Get/set attributes specific to asteroids
		glm::quat GetPatrolAngM(void) const;
		glm::quat GetChaseAngM(void) const;
		float GetChaseRadius(void) const;
		float GetChaseAngle(void) const;
		float GetAttackAngle(void) const;
		void SetPatrolAngM(glm::quat angm);
		void SetChaseAngM(glm::quat angm);
		void SetChaseRadius(float chase_radius);
		void SetChaseAngle(float chase_angle);
		void SetAttackAngle(float attack_angle);

	protected:
		// Angular momentum of asteroid
		glm::quat patrol_angm_;
		glm::quat chase_angm_;
		float chase_radius_;
		float chase_angle_;
		float attack_angle_;

		float chase_timer_;
		float max_chase_timer_;
	};
}

#endif // TurretNode_H_
