#include "air_enemy.h"
#include "utilities.h"
#include <iostream>
namespace game
{

	AirEnemy::AirEnemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : GroundEnemy(name, geometry, material, texture, envmap)
	{

	}
	AirEnemy::~AirEnemy()
	{
	}

	void AirEnemy::Init(void)
	{
		GroundEnemy::Init();

		mid_y_ = 10.f;
		y_offset_[0] = -5.0f;
		y_offset_[1] = 0.0f;
		y_offset_[2] = 5.0f;
		off_index_ = 0;
		off_step_ = 1;
		desired_y_ = mid_y_ + y_offset_[off_index_];

		off_timer_ = 0.0f;
		max_off_timer_ = 5.0f;

		y_speed_ = 2.5f;
	}

	void AirEnemy::Update(float deltaTime)
	{
		switch (state_)
		{
		case(State::Idle):
			std::cout<< GetName() << " " << "Idle" << std::endl;
			StaticEnemy::Idle(deltaTime);
			break;
		case(State::Patrol): 
			std::cout<< GetName() << " " << "Patrol" << std::endl;
			AirEnemy::Patrol(deltaTime);
			break;
		case(State::Chase):  
			std::cout<< GetName() << " " << "Chase" << std::endl;
			AirEnemy::Chase(deltaTime);
			break;
		case(State::Attack): 
			std::cout<< GetName() << " " << "Attack" << std::endl;
			AirEnemy::Attack(deltaTime);
			break;
		case(State::Die):    
			std::cout<< GetName() << " " << "Die" << std::endl; 
			StaticEnemy::Die(deltaTime);
			break;
		}
	}
		
	void AirEnemy::Patrol(float deltaTime)
	{
		//X-z movement covered here. I need to re-work ywards movement
		GroundEnemy::Patrol(deltaTime);

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

				desired_y_ = mid_y_ + y_offset_[off_index_];

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
		if (state_ = State::Chase)
		{
			chase_quat_ = glm::angleAxis(utilities::RandPercent() * glm::pi<float>() * 2.0f, target_->GetUp());
		}
	}
	void AirEnemy::Chase(float deltaTime)
	{
		GroundEnemy::Chase(deltaTime);
		
		desired_y_ = target_->GetPosition().y;

		float y_vel_ = desired_y_ - position_.y;

		if (abs(y_vel_) > y_diff_)
		{
			position_.y += y_speed_ * y_vel_ * deltaTime;
		}
	}

	void AirEnemy::Attack(float deltaTime)
	{

	}
}