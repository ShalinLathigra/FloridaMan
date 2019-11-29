#include "shuriken.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace game
{
	Shuriken::Shuriken(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material)
	{
		active_ = false;
		width_ = .5f;
		forward_ = glm::vec3(0.0f, 0.0f, 1.0f);	//initial forward

		max_duration_timer_ = .25f;
		max_cd_timer_ = .25f;

		duration_timer_ = 0.0f;
		cd_timer_ = 0.0f;

		joint_ = glm::vec3(0.0f);
		orbit_amount_ = glm::angleAxis(glm::pi<float>() / 2.0f, glm::vec3(-1.0f, 0.0f, 0.0f));

		speed = 100.0f;
		
	}

	Shuriken::~Shuriken()
	{
	}

	void Shuriken::setParent(Camera *camera) {
		parent_node_ = camera;
	}

	void Shuriken::SetSpawnPos(glm::vec3 spawn_pos_new) {
		spawn_pos_ = spawn_pos_new;
	}

	void Shuriken::SetForward(glm::vec3 forward_new) {
		forward_ = forward_new;
	}

	void Shuriken::turnOff(void) {
		active_ = false;
	}

	void Shuriken::Update(float deltaTime)
	{
		glm::vec3 fireDirection = glm::normalize(forward_);
		this->position_ += fireDirection * speed * deltaTime;
		distanceTravelled();
		//AdvanceTimers(deltaTime);
		//glm::mat4 fullTransf = CalculateMatrix(parent_node_->GetCurrentViewMatrix());
		//SetPosition(glm::vec3(-0.5f,-1.0f,0));


	}

	void Shuriken::distanceTravelled() {
		float t_distance= glm::length(spawn_pos_ - this->GetPosition());

		if (t_distance > 50.0f) {
			set_toDestroy = true;
		}

	}

	glm::vec3 Shuriken::GetForward(void) const
	{
		return forward_; // Return -forward since the camera coordinate system points in the opposite direction
	}
}