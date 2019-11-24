#include "static_entity.h"
#include "utilities.h"
#include <iostream>
namespace game
{

	StaticEntity::StaticEntity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : Entity(name, geometry, material, texture, envmap)
	{

		patrol_angm_ = glm::angleAxis(glm::pi<float>() / 256.0f, glm::vec3(0.0, 1.0, 0.0));
		chase_angm_ = glm::angleAxis(glm::pi<float>() / 128.0f, glm::vec3(0.0, 1.0, 0.0));
		chase_radius_ = 150.0f;
		chase_angle_ = 0.7f;
		attack_angle_ = 0.975f;
		max_idle_timer_ = 3.0f;
		idle_timer_ = max_idle_timer_;

		max_chase_timer_ = 10.0f;
		chase_timer_ = max_chase_timer_;

	}
	StaticEntity::~StaticEntity()
	{
	}


	void StaticEntity::Update(float deltaTime)
	{
		switch (state_)
		{
		case(State::Idle):
			//std::cout<< GetName() << " " << "Idle" << std::endl;
			StaticEntity::Idle(deltaTime);
			break;
		case(State::Patrol): 
			//std::cout<< GetName() << " " << "Patrol" << std::endl;
			StaticEntity::Patrol(deltaTime); 
			break;
		case(State::Chase):  
			//std::cout<< GetName() << " " << "Chase" << std::endl;
			StaticEntity::Chase(deltaTime); 
			break;
		case(State::Attack): 
			//std::cout<< GetName() << " " << "Attack" << std::endl;
			StaticEntity::Attack(deltaTime); 
			break;
		case(State::Die):    
			//std::cout<< GetName() << " " << "Die" << std::endl; 
			StaticEntity::Die(deltaTime); 
			break;
		}
	}

	void StaticEntity::Idle(float deltaTime)
	{
		idle_timer_ = glm::max(idle_timer_ - deltaTime, 0.0f);
		//std::cout << idle_timer_ << std::endl;
		if (idle_timer_ == 0.0f)
		{
			state_ = State::Patrol;
		}
	}
	void StaticEntity::Patrol(float deltaTime)
	{
		Rotate(patrol_angm_);

		glm::vec3 to_target = target_->GetPosition() - position_;
		float dist_to_target = glm::length(to_target);
		if (dist_to_target < chase_radius_)
		{
			float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));
			if (dot > chase_angle_)
			{
				//std::cout << "PATROL: " << dot << " " << chase_angle_ << std::endl;
				state_ = State::Chase;
				chase_timer_ = max_chase_timer_;
			}
		}

	}
	void StaticEntity::Chase(float deltaTime)
	{
		//Need to rotate such that the dot product increases.

		chase_timer_ = glm::max(chase_timer_ - deltaTime, 0.0f);
		if (chase_timer_ == 0.0f)
		{
			state_ = State::Patrol;
		}

		glm::vec3 to_target = glm::vec3(target_->GetPosition().x, 0.0, target_->GetPosition().z) - glm::vec3(position_.x, 0, position_.z);
		float dist_to_target = glm::length(to_target);
		float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));

		//How to effectively minimize Value?
		glm::vec3 forward_step = utilities::RotateVecByQuat(to_target, chase_angm_);
		glm::vec3 back_step = utilities::RotateVecByQuat(to_target, glm::inverse(chase_angm_));

		float forward_dot = glm::dot(glm::normalize(forward_step), glm::normalize(GetForward()));
		float back_dot = glm::dot(glm::normalize(back_step), glm::normalize(GetForward()));

		//std::cout << "Chase: " << dot << " " << forward_dot << " " << back_dot << " | " << attack_angle_ << std::endl;

		if (dot > attack_angle_)
		{
			state_ = State::Attack;
		}
		else
		{
			if (forward_dot > dot && forward_dot > back_dot)
			{
				Rotate(glm::inverse(chase_angm_));
			}
			else if (back_dot > dot && back_dot > forward_dot)
			{
				Rotate(chase_angm_);
			}
		}

		if (dist_to_target > chase_radius_)
		{
			state_ = State::Patrol;
		}

	}

	void StaticEntity::Attack(float deltaTime)
	{
		glm::vec3 to_target = glm::vec3(target_->GetPosition().x, 0.0, target_->GetPosition().z) - glm::vec3(position_.x, 0, position_.z);
		float dist_to_target = glm::length(to_target);
		float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));

		//Implement Attack Functionality Here!

		if (dot < attack_angle_)
		{
			state_ = State::Chase;
			chase_timer_ = max_chase_timer_;
		}
		
		if (dist_to_target > chase_radius_)
		{
			state_ = State::Patrol;
		}

	}
	void StaticEntity::Die(float deltaTime)
	{
		//Die
	}


	// Getters
	glm::quat StaticEntity::GetChaseAngM(void) const
	{
		return chase_angm_;
	}
	glm::quat StaticEntity::GetPatrolAngM(void) const
	{
		return patrol_angm_;
	}
	float StaticEntity::GetChaseRadius(void) const
	{
		return chase_radius_;
	}
	float StaticEntity::GetChaseAngle(void) const
	{
		return chase_angle_;
	}
	float StaticEntity::GetAttackAngle(void) const
	{
		return attack_angle_;
	}

	// Setters
	void StaticEntity::SetPatrolAngM(glm::quat angm)
	{
		patrol_angm_ = angm;
	}
	void StaticEntity::SetChaseAngM(glm::quat angm)
	{
		chase_angm_ = angm;
	}
	void StaticEntity::SetChaseRadius(float chase_radius)
	{
		chase_radius_ = chase_radius;
	}
	void StaticEntity::SetChaseAngle(float chase_angle)
	{
		chase_angle_ = chase_angle;
	}
	void StaticEntity::SetAttackAngle(float attack_angle)
	{
		attack_angle_ = attack_angle;
	}
}