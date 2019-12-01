#include "shuriken.h"
#include "game.h"
#include "scene_graph.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace game
{
Shuriken::Shuriken(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap)
    : SceneNode(name, geometry, material)
{
    forward_ = glm::vec3(0.0f, 0.0f, 1.0f); //initial forward

    speed = 250.0f;
}

Shuriken::~Shuriken()
{
}

void Shuriken::setParent(Camera *camera)
{
    parent_node_ = camera;
}

void Shuriken::SetSpawnPos(glm::vec3 spawn_pos_new)
{
    spawn_pos_ = spawn_pos_new;
}

void Shuriken::SetForward(glm::vec3 forward_new)
{
    forward_ = forward_new;
}

void Shuriken::turnOff(void)
{
    active_ = false;
}

void Shuriken::Update(float deltaTime)
{
    glm::vec3 fireDirection = glm::normalize(forward_);
    this->position_ += fireDirection * speed * deltaTime;
    distanceTravelled();
    checkIfHit();
}

void Shuriken::distanceTravelled()
{
    float t_distance = glm::length(spawn_pos_ - this->GetPosition());

	if (t_distance > 250.0f)
	{
		set_toDestroy = true;
		while(m_childNodes.size() > 0)
		{
			m_childNodes.pop_back();
		}
    }
}


void Shuriken::checkIfHit()
{
    SceneGraph *o = game_->GetGraph();
    std::vector<SceneNode *> childz = o->GetNode("Root Node")->GetChildren();

	//target_->SetType((target_->GetType() + 1) % 4);

	if (type_ == EntityType::Default)
	{
		for (int i = 0; i < childz.size(); i++)
		{
			if (CheckSphereCollision(childz[i], glm::length(childz[i]->GetScale() + GetScale())) && childz[i]->GetName().find("Entity") != std::string::npos)
			{
				((Entity *)childz[i])->TakeDamage(100);
			}
		}
	}
	else
	{
		Player *target = (Player *)o->GetNode("Player");
		if (CheckSphereCollision(target, 5.0f))
		{
			target->TakeDamage(10);
		}
	}
}

glm::vec3 Shuriken::GetForward(void) const
{
    return forward_; // Return -forward since the camera coordinate system points in the opposite direction
}
} // namespace game