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
		angm_ = glm::angleAxis(glm::pi<float>() / 256.0f, glm::vec3(0.0, 1.0, 0.0));
		chase_radius_ = 100.0f;
		chase_angle_ = 0.7f;
		attack_angle_ = 0.99f;
		max_idle_timer_ = 3.0f;
		idle_timer_ = max_idle_timer_;


		float acc_;	// Acceleration Rate
		float dec_ = 7.5f; // Deceleration Rate

		float vel_ = 0.0f;	// Rate of position change
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
	void GroundEnemy::Patrol(float deltaTime)
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
	void GroundEnemy::Chase(float deltaTime)
	{

	}

	void GroundEnemy::Attack(float deltaTime)
	{
		
	}
	void GroundEnemy::Die(float deltaTime)
	{
		//Die
	}
}