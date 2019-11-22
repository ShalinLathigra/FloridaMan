#include "enemy.h"
#include <iostream>
#include "utilities.h"
namespace game
{
	Enemy::Enemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material, texture, envmap) {
	}


	Enemy::~Enemy()
	{
	}

	void Enemy::Init(void)
	{
		state_ = game::State::Idle;
		forward_ = glm::vec3(0.0, 0.0, 1.0);
		up_ = glm::vec3(0.0, 1.0, 0.0);
		max_idle_timer_ = 10.0f;
		idle_timer_ = max_idle_timer_;
	}

	void Enemy::Update(float deltaTime)
	{
		switch (state_)
		{
		case(State::Idle): Enemy::Idle(deltaTime); break;
		case(State::Patrol): Enemy::Patrol(deltaTime); break;
		case(State::Chase): Enemy::Chase(deltaTime); break;
		case(State::Attack): Enemy::Attack(deltaTime); break;
		case(State::Die): Enemy::Die(deltaTime); break;
		}
	}

	void Enemy::Idle(float deltaTime)
	{
		std::cout << "Idling! " << std::endl;
	}
	void Enemy::Patrol(float deltaTime)
	{
		std::cout << "Patrolling! " << std::endl;
	}
	void Enemy::Chase(float deltaTime)
	{
		std::cout << "Chasing! " << std::endl;
	}
	void Enemy::Attack(float deltaTime)
	{
		std::cout << "Attacking! " << std::endl;
	}
	void Enemy::Die(float deltaTime)
	{
		std::cout << "Dieing! " << std::endl;
	}
	void Enemy::TakeDamage(float amount)
	{
		hp_ -= amount;
		if (hp_ <= 0.0f)
		{
			state_ = State::Die;
		}
	}

	//Getters + Setters
	State Enemy::GetState(void)
	{
		return state_;
	}
	Camera* Enemy::GetTarget(void)
	{
		return target_;
	}
	glm::vec3 Enemy::GetForward(void)
	{
		return utilities::RotateVecByQuat(forward_, orientation_);
	}
	glm::vec3 Enemy::GetUp(void)
	{
		return utilities::RotateVecByQuat(up_, orientation_);
	}
	void Enemy::SetState(State state)
	{
		state_ = state;
	}
	void Enemy::SetTarget(Camera* target)
	{
		target_ = target;
	}
}