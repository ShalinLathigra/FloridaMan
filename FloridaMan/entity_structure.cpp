#include "entity_structure.h"

namespace game {

	EntityStructure::EntityStructure(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) : SceneNode(name, geometry, material, texture, envmap)
	{
	}


	EntityStructure::~EntityStructure()
	{
	}

	void EntityStructure::Update(float deltaTime)
	{
		if (m_ChildCount < MAX_CHILDREN)
		{
			if (m_SpawnTimer <= 0.0f)
			{
//				SceneNode *c = CreateEntity();
				//game_->AddNodeQuad(c);
				m_SpawnTimer = MAX_SPAWN_TIMER;
			}
			else
			{
				m_SpawnTimer = glm::max(m_SpawnTimer - deltaTime, 0.0f);
			}
		}
	}

	void EntityStructure::InitResources(int type, Resource* obj, Resource* mat, Resource* tex, Resource* env)
	{
		m_type = type;
		m_geom = obj;
		m_mat = mat;
		m_tex = tex;
		m_env = env;
	}
	//SceneNode* CreateEntity()
	//{
	//
	//}
} // namespace game
