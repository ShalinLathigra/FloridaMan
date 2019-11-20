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

		y_vel_ = 0.0f;
		max_y_vel_ = 0.0f;
		y_acc_ = 2.5f;

	}

	void AirEnemy::Update(float deltaTime)
	{
		switch (state_)
		{
		case(State::Idle):
			//std::cout<< GetName() << " " << "Idle" << std::endl;
			StaticEnemy::Idle(deltaTime);
			break;
		case(State::Patrol): 
			//std::cout<< GetName() << " " << "Patrol" << std::endl;
			AirEnemy::Patrol(deltaTime);
			break;
		case(State::Chase):  
			//std::cout<< GetName() << " " << "Chase" << std::endl;
			AirEnemy::Chase(deltaTime);
			break;
		case(State::Attack): 
			//std::cout<< GetName() << " " << "Attack" << std::endl;
			AirEnemy::Attack(deltaTime);
			break;
		case(State::Die):    
			//std::cout<< GetName() << " " << "Die" << std::endl; 
			StaticEnemy::Die(deltaTime);
			break;
		}
	}
		
	void AirEnemy::Patrol(float deltaTime)
	{
		//X-z movement covered here. I need to re-work ywards movement
		//GroundEnemy::Patrol(deltaTime);

		if (position_.y != desired_y_)
		{
			std::cout << position_.y << " " << desired_y_ << std::endl;
			// need to move
			float dir;
			if (desired_y_  - position_.y < 0.0f)
			{
				dir = -1.0f;
			}
			else
			{
				dir = 1.0f;
			}

			y_vel_ = utilities::Clamp(y_vel_ + y_acc_ * dir, -max_y_vel_, max_y_vel_);

			position_.y += y_vel_ * deltaTime;
		}
		else
		{
			//Logic for changing the desired level
			//std::cout << off_index_ << " " << off_timer_ << std::endl;
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
	}
	void AirEnemy::Chase(float deltaTime)
	{
		//X-z movement covered here. I need to re-work ywards movement
		GroundEnemy::Chase(deltaTime);
	}

	void AirEnemy::Attack(float deltaTime)
	{
		state_ = State::Chase;
	}
}