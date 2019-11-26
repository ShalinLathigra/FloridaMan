#include "entity_structure.h"
#include <iostream>
#include "game.h"

namespace game {

	EntityStructure::EntityStructure(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material, texture, envmap)
	{
		m_EntityTimer = MAX_SPAWN_TIMER;
		m_EntityCount = 0;
	}


	EntityStructure::~EntityStructure()
	{
	}

	void EntityStructure::InitResources(int type, Resource* obj, Resource* mat, Resource* tex, Resource* env)
	{
		m_type = type;
		m_geom = obj;
		m_mat = mat;
		m_tex = tex;
		m_env = env;

	}

	void EntityStructure::Update(float deltaTime)
	{

		if (m_EntityCount < MAX_CHILDREN)
		{
			if (m_EntityTimer <= 0.0f)
			{
				SceneNode *c = CreateEntity();
				//game_->AddNode(c);
				m_EntityTimer = MAX_SPAWN_TIMER;
			}
			else
			{
				m_EntityTimer = glm::max(m_EntityTimer - deltaTime, 0.0f);
			}
		}
	}
	SceneNode* EntityStructure::CreateEntity()
	{
		Entity* scn;
		glm::vec3 pos, scale;
		std::string mid;

		if (m_type == 0)
		{
			//Spawn at position + 1/2 scale + 1/2 entity scale
			scale = glm::vec3(10);
			pos = GetPosition() + GetScale().y + scale.y / 2.0f;
			mid = std::string("_Turret_");
			scn = new TurretNode(name_ + mid + std::to_string(m_count), m_geom, m_mat, m_tex, m_env);
		}
		else if (m_type == 1)
		{
			//Spawn at position + 1/2 scale + 1/2 entity scale
			scale = glm::vec3(10);
			pos = GetPosition() - GetScale().y / 2.0f + scale.y;
			mid = std::string("_Ground_");
			scn = new GroundEntity(name_ + mid + std::to_string(m_count), m_geom, m_mat, m_tex, m_env);
		}
		else
		{
			//Spawn at position + 1/2 scale + 1/2 entity scale
			scale = glm::vec3(10);
			pos = GetPosition() + GetScale().y + scale.y / 2.0f;
			mid = std::string("_Air_");
			scn = new AirEntity(name_ + mid + std::to_string(m_count), m_geom, m_mat, m_tex, m_env);
		}

		scn->SetPosition(pos);
		scn->SetScale(scale);
		scn->SetOrientation(GetOrientation());
		scn->SetTarget(game_->GetCamera());
		scn->SetGame(game_);
		scn->SetType(m_type);

		return scn;
	}
} // namespace game
