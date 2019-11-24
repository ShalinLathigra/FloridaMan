#include "scene_graph.h"
#include "mine.h"
#include <iostream>
#include "utilities.h"
#include "game.h"
namespace game
{
	Mine::Mine(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material, texture, envmap) {
		state_ = MineState::MineIdle;
		target_set_ = false;
	}


	Mine::~Mine()
	{
	}

	void Mine::Update(float deltaTime)
	{
		switch (state_)
		{
		case(MineState::MineIdle): Mine::Idle(deltaTime); break;
		case(MineState::MineChase): Mine::Chase(deltaTime); break;
		case(MineState::Boom): Mine::Boom(deltaTime); break;
		}

		//std::cout << "My state: "<< GetState() << std::endl;
	}

	void Mine::Idle(float deltaTime)
	{
		SceneGraph *o = game_->GetGraph();
		std::vector<SceneNode*> childz = o->GetNode("Root Node")->GetChildren();

		//std::cout << "No Target" << std::endl;
		for (int i = 0; i < o->GetNode("Root Node")->GetChildren().size(); i++) {
			std::cout << o->GetNode("Root Node")->GetChildren().size() << " " << i << " " << childz[i]->GetName() << std::endl;
			if (childz[i]->GetName().find("Entity") != std::string::npos && target_set_ == false) {
				checkCollision(childz[i]);
			}
		}

	}
	void Mine::Chase(float deltaTime)
	{
		//std::cout << target_->GetName() << std::endl;
		glm::vec3 towardsTarget = glm::normalize(target_->GetPosition() - this->GetPosition());
		accel_ += 0.3f;
		this->position_ += towardsTarget*accel_*deltaTime;

		if (glm::length(target_->GetPosition() - this->GetPosition()) <= 1.0f)
			state_ = MineState::Boom;

		
	}
	void Mine::Boom(float deltaTime)
	{
		//Apply some health damage to enemy here
		//Blow up effects here

		this->set_toDestroy = true;

	}

	void Mine::checkCollision(SceneNode* someEnemy) {
		if (glm::length(someEnemy->GetPosition() - this->position_) <= 50.0f) {

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
		state_ = MineState::MineChase;
	}
}
