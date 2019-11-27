#include "entity.h"
#include <iostream>
#include "utilities.h"
#include "game.h"
namespace game
{
	Entity::Entity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material, texture, envmap)
	{
		death_particles_ = ParticleNode(name + std::string("death"));
		death_part_ = false;
		state_ = game::State::Idle;
		forward_ = glm::vec3(0.0, 0.0, 1.0);
		up_ = glm::vec3(0.0, 1.0, 0.0);
		max_idle_timer_ = 10.0f;
		idle_timer_ = max_idle_timer_;
		max_hp_ = 50.0f;
		hp_ = max_hp_;
	}


	Entity::~Entity()
	{
	}


	void Entity::Update(float deltaTime)
	{
		switch (state_)
		{
		//case(State::Idle): std::cout<<"State: Idle"<<std::endl; break;
		//case(State::Patrol): std::cout << "State: Patrol" << std::endl;  break;
		//case(State::Chase): std::cout << "State: Chase" << std::endl;  break;
		//case(State::Attack): std::cout << "State: Attack" << std::endl;  break;
		//case(State::Die): std::cout << "State: Die" << std::endl;  break;
		}
	}
	void Entity::TakeDamage(float amount)
	{
		hp_ -= amount;
		if (hp_ <= 0.0f)
		{
			if (death_part_ && state_ != State::Die)
			{
				death_particles_.SetScale(glm::vec3(1));
				death_particles_.SetPosition(GetPosition());
				death_particles_.Start();
				std::cout << death_particles_.GetStart() << std::endl;
				game_->AddNode(&death_particles_);
			}
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
	void Entity::SetEndScale(glm::vec3 end_scale)
	{
		end_scale_ = end_scale;
	}
	bool Entity::IsAlive(void)
	{
		return hp_ > 0;
	}
	void Entity::SetDeathEffect(ParticleNode part)
	{
		death_particles_ = part;
		death_part_ = true;
	}

}
