#include "bomb.h"
#include <iostream>
#include "utilities.h"
namespace game
{
	Bomb::Bomb(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material, texture, envmap) {
		state_ = BombState::BombFall;
	}


	Bomb::~Bomb()
	{
	}

	void Bomb::Update(float deltaTime)
	{
		switch (state_)
		{
		case(BombState::BombFall): Bomb::Fall(deltaTime); break;
		case(BombState::BombBoom): Bomb::Boom(deltaTime); break;
		}
	}

	void Bomb::Fall(float deltaTime)
	{
		//glm::vec3 towardsTarget = glm::normalize(target_->GetPosition() - this->GetPosition());
		//this->position_ += towardsTarget * 4.0f*deltaTime;

		//if (glm::length(target_->GetPosition() - this->GetPosition()) <= 1.5f)
		//	state_ = BombState::BombBoom;

		std::cout << "Falling" << std::endl;
		velocity_ += glm::vec3(0, -0.5f, 0);
		this->position_ += velocity_*deltaTime;

		if (this->position_.y <= 0.5) {
			SetState(BombBoom);
		}

	}
	void Bomb::Boom(float deltaTime)
	{
		//Apply some health damage to enemies in a radius here

		this->set_toDestroy = true;
	}


	//Getters + Setters
	BombState Bomb::GetState(void)
	{
		return state_;
	}
	void Bomb::SetState(BombState state)
	{
		state_ = state;
	}
}
