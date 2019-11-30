#include "player.h"
#include "game.h"
#include "resource_manager.h"
#include <iostream>

namespace game
{
Player::Player()
    : SceneNode("Player")
{
}

void Player::PlayerInit(ResourceManager *resMan)
{
    m_pResMan = resMan;
    Resource *geometry = m_pResMan->GetResource("CubeMesh");
    if (geometry->GetType() == PointSet)
    {
        mode_ = GL_POINTS;
    }
    else if (geometry->GetType() == Mesh)
    {
        mode_ = GL_TRIANGLES;
    }

    Resource *material = m_pResMan->GetResource("MetalMaterial");
    array_buffer_ = geometry->GetArrayBuffer();
    element_array_buffer_ = geometry->GetElementArrayBuffer();
    size_ = geometry->GetSize();
    material_ = material->GetResource();
    texture_ = 0;
    envmap_ = 0;
    m_isVisible = true;
    blending_ = false;
    skybox_ = false;
    scale_ = glm::vec3(1.0, 1.0, 1.0);
    Translate(glm::vec3(0.0, 0.0, 0));
    m_ShurikenTimer = 0.0f;
    m_BombTimer = 0.0f;
    m_MineTimer = 0.0f;
}
void Player::Draw(Camera *cam)
{
    SceneNode::Draw(cam);
}

void Player::Update(float deltaTime)
{
    m_ShurikenTimer = glm::max(m_ShurikenTimer - deltaTime, 0.0f);
    m_BombTimer = glm::max(m_BombTimer - deltaTime, 0.0f);
    m_MineTimer = glm::max(m_MineTimer - deltaTime, 0.0f);
}

void Player::Fire(int entityType)
{
    glm::vec3 offset = -game_->GetCamera()->GetUp() * 0.5f;
    switch (entityType)
    {
        case (EntityType::ShurikenProj):
            if (m_ShurikenTimer <= 0.0f)
            {
                //std::cout << "Fire Shuriken!" << std::endl;
                game_->AddNode(game_->CreateEntity(EntityType::ShurikenProj, position_ + offset, glm::vec3(1.0f, 0.25f, 1.0f)));
                m_ShurikenTimer = MAX_SHURIKEN_TIMER;
            }
            break;
        case (EntityType::BombProj):
            if (m_BombTimer <= 0.0f)
            {
                //std::cout << "Fire Bomb!!" << std::endl;
                game_->AddNode(game_->CreateEntity(EntityType::BombProj, position_ + offset, glm::vec3(5.0)));
                m_BombTimer = MAX_BOMB_TIMER;
            }
            break;
        case (EntityType::MineProj):
            if (m_MineTimer <= 0.0f)
            {
                //std::cout << "Fire Mine!!!" << std::endl;
                SceneNode *mine = game_->CreateEntity(EntityType::MineProj, position_ + offset, glm::vec3(5.0));
                mine->SetGame(game_);
                game_->AddNode(mine);
                m_MineTimer = MAX_MINE_TIMER;
            }
            break;
    }
}
} // namespace game