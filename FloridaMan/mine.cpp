#include "mine.h"
#include <iostream>
#include "utilities.h"
namespace game
{
	Mine::Mine(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material, texture, envmap) {
	}


	Mine::~Mine()
	{
	}

	void Mine::Init(void)
	{
		state_ = MineState::MineIdle;
	}

	void Mine::Update(float deltaTime)
	{
		switch (state_)
		{
		case(MineState::MineIdle): Mine::Idle(deltaTime); break;
		case(MineState::MineChase): Mine::Chase(deltaTime); break;
		case(MineState::Boom): Mine::Boom(deltaTime); break;
		}
	}

	void Mine::Idle(float deltaTime)
	{
		std::cout << "Idling! " << std::endl;
	}
	void Mine::Chase(float deltaTime)
	{
		std::cout << "Patrolling! " << std::endl;
	}
	void Mine::Boom(float deltaTime)
	{
		std::cout << "Chasing! " << std::endl;
	}

	//Getters + Setters
	MineState Mine::GetState(void)
	{
		return state_;
	}
	Enemy* Mine::GetTarget(void)
	{
		return target_;
	}
	void Mine::SetState(MineState state)
	{
		state_ = state;
	}
	void Mine::SetTarget(Enemy* target)
	{
		target_ = target;
	}
}
