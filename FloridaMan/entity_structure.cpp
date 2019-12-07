#include "entity_structure.h"
#include "game.h"
#include <iostream>

namespace game
{
EntityStructure::EntityStructure(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap)
    : SceneNode(name, geometry, material, texture, envmap)
{
    death_part_ = ParticleNode("");
    m_EntityTimer = 0.0f;
    m_MaxEntityCount = 3;
    m_EntityCount = 0;
}

EntityStructure::~EntityStructure()
{
}

void EntityStructure::InitResources(int type, Resource *obj, Resource *mat, Resource *tex, Resource *env, ParticleNode part)
{
    m_type = type;
    m_geom = obj;
    m_mat = mat;
    m_tex = tex;
    m_env = env;

    death_part_ = part;

    if (m_type == EntityType::Turret)
        m_MaxEntityCount = 1;
}

void EntityStructure::Update(float deltaTime)
{
	if (game_->GetPlayer()->CheckCollision(this))
	{
		game_->GetPlayer()->SetCollisionEntity(this);
	}
    if (m_EntityCount < m_MaxEntityCount)
    {
        if (m_EntityTimer <= 0.0f)
        {
            SceneNode *c = CreateEntity();
            game_->AddNode(c);
            m_EntityTimer = MAX_SPAWN_TIMER;
            m_EntityCount++;
        }
        else
        {
            m_EntityTimer = glm::max(m_EntityTimer - deltaTime, 0.0f);
        }
    }
}
SceneNode *EntityStructure::CreateEntity()
{
    Entity *scn;
    glm::vec3 pos, scale;
    std::string mid;

    if (m_type == EntityType::Turret)
    {
        //Spawn at position + 1/2 scale + 1/2 entity scale
        scale = glm::vec3(10);
        pos = GetPosition() + glm::vec3(0, GetScale().y + scale.y / 2.0f, 0);
        mid = std::string("_tor_Turret_Entity_");
        scn = new TurretNode(name_ + mid + std::to_string(m_count), m_geom, m_mat, m_tex, m_env);
    }
    else if (m_type == EntityType::Ground)
    {
        //Spawn at position + 1/2 scale + 1/2 entity scale
        scale = glm::vec3(5, 5, 5);
        pos = GetPosition() - GetScale().y / 2.0f + scale.y;
        mid = std::string("_tor_Ground_Entity_");
        scn = new GroundEntity(name_ + mid + std::to_string(m_count), m_geom, m_mat, m_tex, m_env);
    }
    else if (m_type == EntityType::Air)
    {
        //Spawn at position + 1/2 scale + 1/2 entity scale
        scale = glm::vec3(5, 5, 5);
        pos = GetPosition() + glm::vec3(0, GetScale().y + scale.y / 2.0f, 0);
        mid = std::string("_Air_Entity_");
        scn = new AirEntity(name_ + mid + std::to_string(m_count), m_geom, m_mat, m_tex, m_env);
    }

    scn->SetPosition(pos);
    scn->SetScale(glm::vec3(0));
    scn->SetEndScale(scale);
    scn->SetOrientation(GetOrientation());
    scn->SetGame(game_);
    scn->SetType(m_type);
    scn->SetTarget(game_->GetCamera()->GetPlayer());

    scn->SetDeathEffect(death_part_);

    return scn;
}
} // namespace game
