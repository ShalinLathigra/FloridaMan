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
		target_set_ = false;
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
		//std::cout << "No Target" << std::endl;
	}
	void Mine::Chase(float deltaTime)
	{
		//std::cout << "Target found: " << target_->GetName() << std::endl;
		glm::vec3 towardsTarget = glm::normalize(target_->GetPosition() - this->GetPosition());
		this->position_ += towardsTarget*4.0f*deltaTime;

		if (glm::length(target_->GetPosition() - this->GetPosition()) <= 1.5f)
			state_ = MineState::Boom;

		
	}
	void Mine::Boom(float deltaTime)
	{
		std::cout << "REEEEEEE" << std::endl;
		//Apply some health damage to enemy here
		//Blow up effects here

		//SetScale(glm::vec3(0));
		//this->setToDestroy();

	}

	void Mine::checkCollision(SceneNode* someEnemy) {
		if (glm::length(this->position_ - someEnemy->GetPosition()) <= 1.0f) {
			SetTarget(someEnemy);
		}
	}

	//Getters + Setters
	MineState Mine::GetState(void)
	{
		return state_;
	}
	SceneNode* Mine::GetTarget(void)
	{
		return target_;
	}
	void Mine::SetState(MineState state)
	{
		state_ = state;
	}
	void Mine::SetTarget(SceneNode* target)
	{
		target_ = target;
		target_set_ = true;
		state_ = MineChase;
	}
}
