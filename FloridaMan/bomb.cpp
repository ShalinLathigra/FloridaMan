#include "bomb.h"
#include "game.h"
#include "scene_graph.h"
#include "utilities.h"
#include <iostream>
namespace game
{
Bomb::Bomb(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap)
    : SceneNode(name, geometry, material, texture, envmap)
{
    state_ = BombState::BombFall;
    boom_radius_ = 75.0f;
    grav_ = 5.0f;
    velocity_ = glm::vec3(0, 0, 0);
}

Bomb::~Bomb()
{
}

void Bomb::Update(float deltaTime)
{
    switch (state_)
    {
        case (BombState::BombFall):
            Bomb::Fall(deltaTime);
            break;
        case (BombState::BombBoom):
            Bomb::Boom(deltaTime);
            break;
    }
}

void Bomb::Fall(float deltaTime)
{
    velocity_ -= glm::vec3(0, grav_, 0);
    position_ += velocity_ * deltaTime;

    if (this->position_.y <= 0.5)
    {
        SetState(BombBoom);
    }
}
void Bomb::Boom(float deltaTime)
{
    if (!set_toDestroy)
    {
        set_toDestroy = true;

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
        game_->AddNode(&death_part_);
        death_part_.SetScale(glm::vec3(1));
        death_part_.SetPosition(GetPosition());
        death_part_.Start();
    }
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
void Bomb::SetDeathPart(ParticleNode part)
{
    death_part_ = part;
}
} // namespace game
