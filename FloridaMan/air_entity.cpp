#include "air_entity.h"
#include "utilities.h"
#include <iostream>
namespace game
{

	AirEntity::AirEntity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : GroundEntity(name, geometry, material, texture, envmap)
	{
		attack_angle_ = 0.99f;
		attack_radius_ = 30.0f;
		chase_angm_ = glm::angleAxis(glm::pi<float>() / 512.0f, glm::vec3(0, 1, 0));

		mid_y_ = 10.f;
		y_offset_[0] = -5.0f;
		y_offset_[1] = 0.0f;
		y_offset_[2] = 5.0f;
		off_index_ = 0;
		off_step_ = 1;
		desired_y_ = mid_y_ + y_offset_[off_index_];

		off_timer_ = 0.0f;
		max_off_timer_ = 2.5f;

		y_speed_ = 0.75f;
		speed_ = 9.0f;
	}
	AirEntity::~AirEntity()
	{
	}

	void AirEntity::Update(float deltaTime)
	{
		switch (state_)
		{
		case(State::Idle):
			//std::cout<< GetName() << " " << "Idle" << std::endl;
			StaticEntity::Idle(deltaTime);
			break;
		case(State::Patrol): 
			//std::cout<< GetName() << " " << "Patrol" << std::endl;
			AirEntity::Patrol(deltaTime);
			break;
		case(State::Chase):  
			//std::cout<< GetName() << " " << "Chase" << std::endl;
			AirEntity::Chase(deltaTime);
			break;
		case(State::Attack): 
			//std::cout<< GetName() << " " << "Attack" << std::endl;
			AirEntity::Attack(deltaTime);
			break;
		case(State::Die):    
			//std::cout<< GetName() << " " << "Die" << std::endl; 
			StaticEntity::Die(deltaTime);
			break;
		}
	}
		
	void AirEntity::Patrol(float deltaTime)
	{
		//X-z movement covered here. I need to re-work ywards movement
		GroundEntity::Patrol(deltaTime);

		AssessYOffset(deltaTime);
		MaintainY(target_->GetPosition(), deltaTime);

		SetState();
	}
	void AirEntity::Chase(float deltaTime)
	{
		GroundEntity::Chase(deltaTime);

		AssessYOffset(deltaTime);
		MaintainY(target_->GetPosition(), deltaTime);

		SetState();
	}

	void AirEntity::Attack(float deltaTime)
	{
		glm::vec3 right = glm::normalize(glm::cross(GetForward(), GetUp()));
		Translate(5.0f * right * deltaTime);
		StaticEntity::Chase(deltaTime);

		AssessYOffset(deltaTime);
		MaintainY(target_->GetPosition(), deltaTime);
		GroundEntity::Attack(deltaTime);
		SetState();
		
		// should slow down movement gradually
		// Check angle to player, if dot between dir to player and forward < attack_radius, turn a bit.
		// translate to the right or left
	}

	void AirEntity::SetState()
	{

		glm::vec3 to_target = target_->GetPosition() - position_;
		float dist_to_target = glm::length(to_target);
		if (dist_to_target > chase_radius_)
		{
			state_ = State::Patrol;
		}
		else if (dist_to_target < attack_radius_)
		{
			state_ = State::Attack;
		}
		else
		{
			state_ = State::Chase;
		}
	}

	void AirEntity::MaintainY(glm::vec3 target_pos, float deltaTime)
	{
		desired_y_ = target_pos.y + y_offset_[off_index_];

		float y_vel = desired_y_ - position_.y;

		y_vel = utilities::Clamp(y_vel, -y_speed_, y_speed_);

		if (y_vel * y_vel > y_diff_ * y_diff_)
		{
			position_.y += y_speed_ * y_vel * deltaTime;
		}
	}

	void AirEntity::AssessYOffset(float deltaTime)
	{
		if (position_.y != desired_y_)
		{
			float y_vel_ = y_speed_ * (desired_y_ - position_.y);

			if (abs(y_vel_) < .025f)
			{
				position_.y = desired_y_;
			}
			else
			{
				position_.y += y_vel_ * deltaTime;
			}
		}
		else
		{
			//Logic for changing the desired level
			off_timer_ = glm::max(off_timer_ - deltaTime, 0.0f);
			if (off_timer_ == 0.0f)
			{
				off_timer_ = max_off_timer_;
				off_index_ += off_step_;

				if (off_index_ == 0)
				{
					off_step_ *= -1;
				}
				else if (off_index_ == 2)
				{
					off_step_ *= -1;
				}
			}
		}
	}
}