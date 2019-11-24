#include "entity.h"
#include <iostream>
#include "utilities.h"
namespace game
{
	Entity::Entity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material, texture, envmap)
	{
		state_ = game::State::Idle;
		forward_ = glm::vec3(0.0, 0.0, 1.0);
		up_ = glm::vec3(0.0, 1.0, 0.0);
		max_idle_timer_ = 10.0f;
		idle_timer_ = max_idle_timer_;
	}


	Entity::~Entity()
	{
	}


	void Entity::Update(float deltaTime)
	{
		switch (state_)
		{
		case(State::Idle): Entity::Idle(deltaTime); break;
		case(State::Patrol): Entity::Patrol(deltaTime); break;
		case(State::Chase): Entity::Chase(deltaTime); break;
		case(State::Attack): Entity::Attack(deltaTime); break;
		case(State::Die): Entity::Die(deltaTime); break;
		}
	}

	void Entity::Idle(float deltaTime)
	{
		std::cout << "Idling! " << std::endl;
	}
	void Entity::Patrol(float deltaTime)
	{
		std::cout << "Patrolling! " << std::endl;
	}
	void Entity::Chase(float deltaTime)
	{
		std::cout << "Chasing! " << std::endl;
	}
	void Entity::Attack(float deltaTime)
	{
		std::cout << "Attacking! " << std::endl;
	}
	void Entity::Die(float deltaTime)
	{
		std::cout << "Dieing! " << std::endl;
	}
	void Entity::TakeDamage(float amount)
	{
		hp_ -= amount;
		if (hp_ <= 0.0f)
		{
			state_ = State::Die;
		}
	}

	//Getters + Setters
	State Entity::GetState(void)
	{
		return state_;
	}
	Camera* Entity::GetTarget(void)
	{
		return target_;
	}
	glm::vec3 Entity::GetForward(void)
	{
		return utilities::RotateVecByQuat(forward_, orientation_);
	}
	glm::vec3 Entity::GetUp(void)
	{
		return utilities::RotateVecByQuat(up_, orientation_);
	}
	void Entity::SetState(State state)
	{
		state_ = state;
	}
	void Entity::SetTarget(Camera* target)
	{
		target_ = target;
	}
}
