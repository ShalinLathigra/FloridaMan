#include "enemy.h"

namespace game {

	Enemy::Enemy(const std::string name, const Resource *geometry, const Resource *material) : SceneNode(name, geometry, material) 
	{

	}


	Enemy::~Enemy() 
	{

	}


	void Enemy::Update(float deltaTime)
	{
		switch (current_state_)
		{
		case(State::Patrol): Patrol(deltaTime);break;
		case(State::Chase): Chase(deltaTime);break;
		case(State::Attack): Attack(deltaTime);break;
		case(State::Die): Die(deltaTime);break;
		}
	}

	void Enemy::Patrol(float deltaTime)
	{
		// Rotate around in arbitrary direction, monitor if angle to target is <=  detect angle, set state Chase
		// If HP == 0, Set State, Die


	}
	
	void Enemy::Chase(float deltaTime)
	{
		// Rotate around, if abs(angle to target + some offset) < some value, Attack
		// If dist to player < some value, Patrol
		// If HP == 0, Set State, Die
	}
	
	void Enemy::Attack(float deltaTime)
	{
		// If abs(angle to target + some offset) > attack_angle , Chase
		// If HP == 0, Set State, Die
	}
	
	void Enemy::Die(float deltaTime)
	{
		// Do Nothing.
	}

	glm::quat Enemy::GetPatrolAng(void)
	{
		return patrol_ang_;
	}

	void Enemy::SetPatrolAng(glm::quat ang)
	{
		patrol_ang_ = ang;
	}

	void Enemy::SetTarget(const glm::vec3 & target)
	{
		target_ = target;
	}

} // namespace game
