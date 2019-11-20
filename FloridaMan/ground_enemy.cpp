#include "ground_enemy.h"
#include "utilities.h"
#include <iostream>
namespace game
{

	GroundEnemy::GroundEnemy(const std::string name, const Resource *geometry, const Resource *material) : StaticEnemy(name, geometry, material)
	{

	}
	GroundEnemy::~GroundEnemy()
	{
	}

	void GroundEnemy::Init(void)
	{
		StaticEnemy::Init();
		patrol_angm_ = glm::angleAxis(glm::pi<float>() / 512.0f, glm::vec3(0.0, 1.0, 0.0));
		chase_angm_ = glm::angleAxis(glm::pi<float>() / 256.0f, glm::vec3(0.0, 1.0, 0.0));
		chase_radius_ = 120.0f;
		brake_radius_ = 30.0f;
		chase_angle_ = 0.7f;
		attack_angle_ = 0.99f;
		max_idle_timer_ = 3.0f;
		idle_timer_ = max_idle_timer_;


		acc_ = 2.5f;	// Acceleration Rate
		dec_ = 4.5f; // Deceleration Rate

		vel_ = 0.0f;	// Rate of position change
		max_vel_ = 15.0f;	// Rate of position change
	}

	void GroundEnemy::Update(float deltaTime)
	{
		switch (state_)
		{
		case(State::Idle):
			//std::cout<< GetName() << " " << "Idle" << std::endl;
			GroundEnemy::Idle(deltaTime);
			break;
		case(State::Patrol): 
			//std::cout<< GetName() << " " << "Patrol" << std::endl;
			GroundEnemy::Patrol(deltaTime);
			break;
		case(State::Chase):  
			//std::cout<< GetName() << " " << "Chase" << std::endl;
			std::cout << GetName() << " " << "Chase";
			GroundEnemy::Chase(deltaTime);
			break;
		case(State::Attack): 
			//std::cout<< GetName() << " " << "Attack" << std::endl;
			GroundEnemy::Attack(deltaTime);
			break;
		case(State::Die):    
			//std::cout<< GetName() << " " << "Die" << std::endl; 
			GroundEnemy::Die(deltaTime);
			break;
		}
	}

	void GroundEnemy::Idle(float deltaTime)
	{
		idle_timer_ = glm::max(idle_timer_ - deltaTime, 0.0f);
		std::cout << idle_timer_ << std::endl;
		if (idle_timer_ == 0.0f)
		{
			state_ = State::Patrol;
		}
	}
	/*
	Casting Time: 1 Bonus Action
	Duration:  1 min. (Concentration)

	Choose one damage type from: acid, cold, fire, lightning, poison, or thunder.
	When you cast the spell you imbue a simple or martial melee weapon, or four pieces of ammunition with your chosen damage type.
	Each time you hit with an imbued strike, add 1d4 of your chosen damage type to the damage roll.

	This spell ends when: you lose concentration, have hit four times with an imbued melee weapon, or have used all four pieces of imbued ammunition, or the duration ends.

	When cast at a higher level, you can strike 2 more times per level above 1st. (6 at second level, 8 at third, 10 at 4th, 12 at 6th).
	*/

	void GroundEnemy::Patrol(float deltaTime)
	{
		Rotate(patrol_angm_);


		if (vel_ < max_vel_ * 2.0f / 3.0f)
		{
			vel_ = glm::min(vel_ + acc_, max_vel_);
		}
		Translate(GetForward() * vel_ * deltaTime);

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
	void GroundEnemy::Chase(float deltaTime)
	{
		// Need to know how hard to turn.
		// If HARD, slow down while turning,.

		// Look at dot product of forward and to_target
		// If <= 0, then need to turn hard, else, turn soft
		
		glm::vec3 to_target = target_->GetPosition() - position_;
		glm::vec3 right = glm::cross(GetForward(), GetUp());

		float dist_to_target = glm::length(to_target);
		float forward_dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));	// Near 0 = Turn Hard. Near 1 = Don't Turn. In between = Turn Soft.
		float right_dot = glm::dot(glm::normalize(to_target), glm::normalize(right));	// > 0 = Go Right, < 0 = Go Left

		//Determine Turning
		glm::quat rot;

		if (forward_dot < 0.5)
		{
			rot = chase_angm_;
		}
		else if (forward_dot < 0.95)
		{
			rot = patrol_angm_;
		}

		if (right_dot > 0.0f)
		{
			rot = glm::inverse(rot);
		}


		if (dist_to_target < brake_radius_ / 2.0f)
		{
			vel_ = glm::max(vel_ - dec_, 0.0f);
			state_ = State::Attack;
		}
		else 
		{
			if (dist_to_target > chase_radius_)
			{
				state_ = State::Patrol;
			}
			else if (dist_to_target < chase_radius_)
			{
				vel_ = glm::min(vel_ + acc_, max_vel_);
			}
			Rotate(rot);
		}

		Translate(GetForward() * vel_ * deltaTime);
	}

	void GroundEnemy::Attack(float deltaTime)
	{
		
	}
	void GroundEnemy::Die(float deltaTime)
	{
		//Die
	}
}