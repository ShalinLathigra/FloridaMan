#include "air_enemy.h"
#include "utilities.h"
#include <iostream>
namespace game
{

	AirEnemy::AirEnemy(const std::string name, const Resource *geometry, const Resource *material) : GroundEnemy(name, geometry, material)
	{

	}
	AirEnemy::~AirEnemy()
	{
	}

	void AirEnemy::Init(void)
	{
		GroundEnemy::Init();

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
	}
	void AirEnemy::Chase(float deltaTime)
	{
		//X-z movement covered here. I need to re-work ywards movement
		GroundEnemy::Chase(deltaTime);
	}

	void AirEnemy::Attack(float deltaTime)
	{
	}
}