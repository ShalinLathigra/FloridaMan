#include "mine.h"
#include "game.h"
#include "scene_graph.h"
#include "utilities.h"
#include <iostream>
namespace game
{
	Mine::Mine(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap)
		: SceneNode(name, geometry, material, texture, envmap)
	{
		state_ = MineState::MineIdle;
		target_set_ = false;
		boom_radius_ = 37.5f;
		chase_radius_ = 50.5f;
		acc_ = 0.5f;
		vel_ = 5.0f;
	}

	Mine::~Mine()
	{
	}

	void Mine::Update(float deltaTime)
	{
		switch (state_)
		{
		case (MineState::MineIdle):
			Mine::Idle(deltaTime);
			break;
		case (MineState::MineChase):
			Mine::Chase(deltaTime);
			break;
		case (MineState::Boom):
			Mine::Boom(deltaTime);
			break;
		}
	}

	void Mine::Idle(float deltaTime)
	{
		SceneGraph *o = game_->GetGraph();
		std::vector<SceneNode *> childz = o->GetNode("Root Node")->GetChildren();

		for (int i = 0; i < childz.size(); i++)
		{
			if (childz[i]->GetName().find("Entity") != std::string::npos && target_set_ == false)
			{
				SetTarget(childz[i]);
			}
		}
	}
	void Mine::Chase(float deltaTime)
	{
		glm::vec3 towardsTarget = glm::normalize(target_->GetPosition() - this->GetPosition());
		vel_ += acc_;
		Translate(towardsTarget * vel_ * deltaTime);

		if (glm::length(target_->GetPosition() - this->GetPosition()) <= 1.0f)
			state_ = MineState::Boom;
	}
	void Mine::Boom(float deltaTime)
	{
		//Apply some health damage to enemy here
		//Blow up effects here

		set_toDestroy = true;
		for (int i = 0; i < m_childNodes.size(); i++)
		{
			RemoveChildAt(0);
		}
		SceneGraph *o = game_->GetGraph();
		std::vector<SceneNode *> childz = o->GetNode("Root Node")->GetChildren();

		for (int i = 0; i < childz.size(); i++)
		{
			if (childz[i]->GetName().find("Entity") != std::string::npos &&
				CheckSphereCollision(childz[i], boom_radius_))
			{
				((Entity *)childz[i])->TakeDamage(100);
			}
		}
	}

	//Getters + Setters
	MineState Mine::GetState(void)
	{
		return state_;
	}
	SceneNode *Mine::GetTarget(void)
	{
		return target_;
	}
	void Mine::SetState(MineState state)
	{
		state_ = state;
	}
	void Mine::SetTarget(SceneNode *target)
	{
		if (CheckSphereCollision(target, chase_radius_))
		{
			target_ = target;
			target_set_ = true;
			state_ = MineState::MineChase;
		}
	}
	void Mine::InitAltMat(ResourceManager *resman_)
	{
		alt_mat_ = resman_->GetResource("ForceMaterial")->GetResource();
	}
	void Mine::Draw(Camera *camera) 
	{
		SceneNode::Draw(camera);

		glm::vec3 temp = GetScale();

		SetScale(glm::vec3(boom_radius_));
		if (state_ == MineState::MineIdle)
			SetScale(glm::vec3(chase_radius_));


		// Select blending or not
		// Disable z-buffer
		glDisable(GL_DEPTH_TEST);

		// Enable blending
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Simpler form
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);

		// Select proper material (shader program)
		glUseProgram(alt_mat_);

		// Set geometry to draw
		glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

		// Set globals for camera
		camera->SetupShader(alt_mat_);

		// Set world matrix and other shader input variables
		SetupShader(alt_mat_, camera);

		// Draw geometry
		if (mode_ == GL_POINTS)
		{
			glDrawArrays(mode_, 0, size_);
		}
		else
		{
			glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
		}



		SetScale(temp);

	}
} // namespace game
