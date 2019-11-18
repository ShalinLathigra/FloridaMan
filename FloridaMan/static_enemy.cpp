#include "static_enemy.h"
#include "utilities.h"
#include <iostream>
namespace game
{

	StaticEnemy::StaticEnemy(const std::string name, const Resource *geometry, const Resource *material) : Enemy(name, geometry, material) 
	{

	}
	StaticEnemy::~StaticEnemy()
	{
	}

	void StaticEnemy::Init(void)
	{
		Enemy::Init();
		angm_ = glm::angleAxis(glm::pi<float>() / 256.0f, glm::vec3(0.0, 1.0, 0.0));
		chase_radius_ = 25.0f;
		chase_angle_ = 0.7f;
		attack_angle_ = 0.99f;
	}

	void StaticEnemy::Update(float deltaTime)
	{
		switch (state_)
		{
		case(State::Patrol): 
			//std::cout<< "Patrol" << std::endl; 
			StaticEnemy::Patrol(deltaTime); 
			break;
		case(State::Chase):  
			//std::cout<< "Chase" << std::endl; 
			StaticEnemy::Chase(deltaTime); 
			break;
		case(State::Attack): 
			//std::cout<< "Attack" << std::endl; 
			StaticEnemy::Attack(deltaTime); 
			break;
		case(State::Die):    
			//std::cout<< "Die" << std::endl; 
			StaticEnemy::Die(deltaTime); 
			break;
		}
	}

	void StaticEnemy::Patrol(float deltaTime)
	{
		Rotate(angm_);

		glm::vec3 to_target = target_->GetPosition() - position_;
		float dist_to_target = glm::length(to_target);
		
		if (dist_to_target < chase_radius_)
		{
			float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));
			if (dot > chase_angle_)
			{
				state_ = State::Chase;
			}
		}

	}
	void StaticEnemy::Chase(float deltaTime)
	{
		//Need to rotate such that the dot product increases.
		
		glm::vec3 to_target = target_->GetPosition() - position_;
		float dist_to_target = glm::length(to_target);
		float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));

		//How to effectively minimize Value?
		glm::vec3 forward_step = utilities::RotateVecByQuat(to_target, angm_);
		glm::vec3 back_step = utilities::RotateVecByQuat(to_target, glm::inverse(angm_));

		float forward_dot = glm::dot(glm::normalize(forward_step), glm::normalize(GetForward()));
		float back_dot = glm::dot(glm::normalize(back_step), glm::normalize(GetForward()));
		
		if (dot < attack_angle_)
		{
			if (forward_dot < dot)
			{
				Rotate(angm_);
			}
			else if (back_dot < dot)
			{
				Rotate(glm::inverse(angm_));
			}
		}
		else
		{
			state_ = State::Attack;
		}

		if (dist_to_target > chase_radius_)
		{
			state_ = State::Patrol;
		}
	}

	void StaticEnemy::Attack(float deltaTime)
	{
		glm::vec3 to_target = target_->GetPosition() - position_;
		float dist_to_target = glm::length(to_target);
		float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));

		//Implement Attack Functionality Here!

		if (dot < attack_angle_)
		{
			state_ = State::Chase;
		}
		
		if (dist_to_target > chase_radius_)
		{
			state_ = State::Patrol;
		}

	}
	void StaticEnemy::Die(float deltaTime)
	{
		//Die
	}


	// Getters
	glm::quat StaticEnemy::GetAngM(void) const
	{
		return angm_;
	}
	float StaticEnemy::GetChaseRadius(void) const
	{
		return chase_radius_;
	}
	float StaticEnemy::GetChaseAngle(void) const
	{
		return chase_angle_;
	}
	float StaticEnemy::GetAttackAngle(void) const
	{
		return attack_angle_;
	}

	// Setters
	void StaticEnemy::SetAngM(glm::quat angm)
	{
		angm_ = angm;
	}
	void StaticEnemy::SetChaseRadius(float chase_radius)
	{
		chase_radius_ = chase_radius;
	}
	void StaticEnemy::SetChaseAngle(float chase_angle)
	{
		chase_angle_ = chase_angle;
	}
	void StaticEnemy::SetAttackAngle(float attack_angle)
	{
		attack_angle_ = attack_angle;
	}
}