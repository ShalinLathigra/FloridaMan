#include "player.h"
#include "game.h"
#include "resource_manager.h"
#include <iostream>

namespace game
{
Player::Player()
    : SceneNode("Player")
{
    m_pCollisionEntity = nullptr;
}

void Player::PlayerInit(ResourceManager *resMan)
{
    m_pResMan = resMan;
    Resource *geometry = m_pResMan->GetResource("SphereMesh");
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
    m_Ramming = false;
    scale_ = glm::vec3(1.0, 1.0, 1.0);
    Translate(glm::vec3(0.0, 0.0, 0));
    m_ShurikenTimer = 0.0f;
    m_BombTimer = 0.0f;
    m_MineTimer = 0.0f;
    m_HP = 500.0f;
    m_speed = 0.0;
    m_maxSpeed = 300.0;
}
void Player::Draw(Camera *cam)
{
    SceneNode::Draw(cam);
}
void Player::SetCollisionEntity(SceneNode *collisionEntity, SceneNode *nodeHit)
{
    m_pCollisionEntity = collisionEntity;
    m_pNodeHit = nodeHit;
}

float Player::GetHP()
{
    return m_HP;
}

void Player::Accelerate(float speed)
{
    m_speed += speed;
    if (m_speed > 0.0f)
    {
        m_speed = 0.0f;
    }
    else if (m_speed < -m_maxSpeed)
    {
        m_speed = -m_maxSpeed;
    }
}

void Player::SetPart(ParticleNode *part)
{
    m_part = *part;
}

void Player::SetCamera(Camera *cam)
{
    m_pCamera = cam;
}

void Player::Ram()
{
    if (m_RamCooldown <= 0)
    {
        m_Ramming = true;
        m_RamTimer = 1.5f;
        m_tempSpeed = m_speed;
        m_speed = -600.0f;
    }
}

void Player::Update(float deltaTime)
{
    m_ShurikenTimer = glm::max(m_ShurikenTimer - deltaTime, 0.0f);
    m_BombTimer = glm::max(m_BombTimer - deltaTime, 0.0f);
    m_MineTimer = glm::max(m_MineTimer - deltaTime, 0.0f);
    m_RamCooldown = glm::max(m_RamCooldown - deltaTime, 0.0f);
    m_RamTimer = glm::max(m_RamTimer - deltaTime, 0.0f);
    if (m_Ramming && m_RamTimer <= 0)
    {
        m_Ramming = false;
        m_speed = m_tempSpeed;
        m_RamCooldown = 10.0f;
    }
    else if (m_Ramming)
    {
        m_speed = -600;
    }

    m_pCamera->move(m_speed * deltaTime);
    for (std::vector<SceneNode *>::iterator iter = m_childNodes.begin(); iter != m_childNodes.end(); iter++)
    {
        (*iter)->Update(deltaTime);
    }
}

void Player::Fire(int entityType)
{
    glm::vec3 offset = -game_->GetCamera()->GetUp() * 0.5f;
    SceneNode *atk;
    switch (entityType)
    {
        case (EntityType::ShurikenProj):
            if (m_ShurikenTimer <= 0.0f)
            {
                glm::vec3 start_pos = position_ + offset;
                atk = game_->CreateEntity(EntityType::ShurikenProj, start_pos, glm::vec3(0));
                atk->SetType(EntityType::Default);
                atk->SetOrientation(GetOrientation());
                ((Shuriken *)atk)->SetForward(utilities::RotateVecByQuat(glm::vec3(0, 0, -1), GetOrientation()));
                ((Shuriken *)atk)->SetSpawnPos(start_pos + offset);
                ((Shuriken *)atk)->SetSpawnVel(-m_speed);
                game_->AddNode(atk);
                m_ShurikenTimer = MAX_SHURIKEN_TIMER;
            }
            break;
        case (EntityType::BombProj):
            if (m_BombTimer <= 0.0f)
            {
                atk = game_->CreateEntity(EntityType::BombProj, position_ + offset, glm::vec3(5.0));
                atk->SetType(EntityType::Default);
                atk->SetOrientation(glm::normalize(glm::angleAxis(utilities::RandPercent() * glm::pi<float>() * 2.0f, glm::vec3(1, 1, -1))));
                ((Bomb *)atk)->SetDeathPart(m_part);
                game_->AddNode(atk);
                m_BombTimer = MAX_BOMB_TIMER;
            }
            break;
        case (EntityType::MineProj):
            if (m_MineTimer <= 0.0f)
            {
                atk = game_->CreateEntity(EntityType::MineProj, position_ + offset, glm::vec3(5.0));
                ((Mine *)atk)->InitAltMat(m_pResMan);
                atk->SetGame(game_);
                atk->SetType(EntityType::Default);
                ((Mine *)atk)->SetDeathPart(m_part);
                game_->AddNode(atk);
                m_MineTimer = MAX_MINE_TIMER;
            }
            break;
    }
}
void Player::TakeDamage(float amount)
{
    m_HP -= amount;
    std::cout << "Ouch! Remaining HP: " << m_HP << std::endl;
    if (m_HP <= 0.0f)
    {
        std::cout << "I AM DECEASED!!!" << std::endl;
    }
}
} // namespace game