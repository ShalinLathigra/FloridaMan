#include "turret_node.h"
#include "game.h"
#include "utilities.h"
#include <iostream>
namespace game
{
TurretNode::TurretNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap)
    : Entity(name, geometry, material, texture, envmap)
{
    patrol_angm_ = glm::angleAxis(glm::pi<float>() / 256.0f, glm::vec3(0.0, 1.0, 0.0));
    chase_angm_ = glm::angleAxis(glm::pi<float>() / 128.0f, glm::vec3(0.0, 1.0, 0.0));

    chase_radius_ = 300.0f;
    chase_angle_ = 0.3f;

    attack_angle_ = 0.9f;
    max_idle_timer_ = 3.0f;
    idle_timer_ = max_idle_timer_;

    max_chase_timer_ = 10.0f;
    chase_timer_ = max_chase_timer_;

    max_attack_timer_ = 2.5f;
    attack_timer_ = max_attack_timer_;
}
TurretNode::~TurretNode()
{
}
void TurretNode::Update(float deltaTime)
{
    SceneNode *nodeHit;
    if (game_->GetPlayer()->CheckCollision(this, &nodeHit))
    {
        game_->GetPlayer()->SetCollisionEntity(this, nodeHit);
    }

    switch (state_)
    {
        case (State::Idle):
            TurretNode::Idle(deltaTime);
            break;
        case (State::Patrol):
            TurretNode::Patrol(deltaTime);
            break;
        case (State::Chase):
            TurretNode::Chase(deltaTime);
            break;
        case (State::Attack):
            TurretNode::Attack(deltaTime);
            break;
        case (State::Die):
            TurretNode::Die(deltaTime);
            break;
    }

    if (hp_ <= 0.0f)
    {
        state_ = State::Die;
    }
}

void TurretNode::Idle(float deltaTime)
{
    glm::vec3 grow_dir = glm::normalize(end_scale_ - scale_);
    float step = glm::length(end_scale_ - scale_) / max_idle_timer_;

    scale_ += grow_dir * step * deltaTime;

    //This will be the spawning Animation
    idle_timer_ = glm::max(idle_timer_ - deltaTime, 0.0f);
    if (idle_timer_ == 0.0f)
    {
        state_ = State::Patrol;
    }
}
void TurretNode::Patrol(float deltaTime)
{
    Rotate(patrol_angm_);

    glm::vec3 to_target = target_->GetPosition() - position_;
    float dist_to_target = glm::length(to_target);
    if (dist_to_target < chase_radius_)
    {
        float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));
        if (dot > chase_angle_)
        {
            state_ = State::Chase;
            chase_timer_ = max_chase_timer_;
        }
    }
}
void TurretNode::Chase(float deltaTime)
{
    //Need to rotate such that the dot product increases.

    chase_timer_ = glm::max(chase_timer_ - deltaTime, 0.0f);
    if (chase_timer_ == 0.0f)
    {
        state_ = State::Patrol;
    }

    glm::vec3 to_target = glm::vec3(target_->GetPosition().x, 0.0, target_->GetPosition().z) - glm::vec3(position_.x, 0, position_.z);
    float dist_to_target = glm::length(to_target);
    float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));

    //How to effectively minimize Value?
    glm::vec3 forward_step = utilities::RotateVecByQuat(to_target, chase_angm_);
    glm::vec3 back_step = utilities::RotateVecByQuat(to_target, glm::inverse(chase_angm_));

    float forward_dot = glm::dot(glm::normalize(forward_step), glm::normalize(GetForward()));
    float back_dot = glm::dot(glm::normalize(back_step), glm::normalize(GetForward()));

    if (dot > attack_angle_)
    {
        state_ = State::Attack;
    }
    else
    {
        if (forward_dot > dot && forward_dot > back_dot)
        {
            Rotate(glm::inverse(chase_angm_));
        }
        else if (back_dot > dot && back_dot > forward_dot)
        {
            Rotate(chase_angm_);
        }
    }

    if (dist_to_target > chase_radius_)
    {
        state_ = State::Patrol;
    }
}

void TurretNode::Attack(float deltaTime)
{
    glm::vec3 to_target = glm::vec3(target_->GetPosition().x, 0.0, target_->GetPosition().z) - glm::vec3(position_.x, 0, position_.z);
    float dist_to_target = glm::length(to_target);
    float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));

    if (attack_timer_ <= 0.0f)
    {
        TurretNode::InstantiateAttack();
        attack_timer_ = max_attack_timer_;
    }
    else
    {
        attack_timer_ = glm::max(attack_timer_ - deltaTime, 0.0f);
    }

    if (dot < attack_angle_)
    {
        state_ = State::Chase;
        chase_timer_ = max_chase_timer_;
    }
    if (dist_to_target > chase_radius_)
    {
        state_ = State::Patrol;
    }
}
void TurretNode::Die(float deltaTime)
{
    //Die
    glm::vec3 grow_dir = glm::normalize(-scale_);

    scale_ += grow_dir * 25.0f * deltaTime;
    scale_.x = glm::max(scale_.x, 0.0f);
    scale_.y = glm::max(scale_.y, 0.0f);
    scale_.z = glm::max(scale_.z, 0.0f);

    if (scale_.x <= 0 || scale_.y <= 0 || scale_.z <= 0)
    {
        set_toDestroy = true;
        AirEntity *air = dynamic_cast<AirEntity *>(this);
        GroundEntity *ground = dynamic_cast<GroundEntity *>(this);

        if (air)
        {
            game_->AirshipKilled();
        }
        else if (ground)
        {
            game_->CybertruckKilled();
        }
        else
        {
            game_->TurretKilled();
        }
    }
}

// Getters
glm::quat TurretNode::GetChaseAngM(void) const
{
    return chase_angm_;
}
glm::quat TurretNode::GetPatrolAngM(void) const
{
    return patrol_angm_;
}
float TurretNode::GetChaseRadius(void) const
{
    return chase_radius_;
}
float TurretNode::GetChaseAngle(void) const
{
    return chase_angle_;
}
float TurretNode::GetAttackAngle(void) const
{
    return attack_angle_;
}
// Setters
void TurretNode::SetPatrolAngM(glm::quat angm)
{
    patrol_angm_ = angm;
}
void TurretNode::SetChaseAngM(glm::quat angm)
{
    chase_angm_ = angm;
}
void TurretNode::SetChaseRadius(float chase_radius)
{
    chase_radius_ = chase_radius;
}
void TurretNode::SetChaseAngle(float chase_angle)
{
    chase_angle_ = chase_angle;
}
void TurretNode::SetAttackAngle(float attack_angle)
{
    attack_angle_ = attack_angle;
}
void TurretNode::InstantiateAttack(void)
{
    Shuriken *atk = (Shuriken *)game_->CreateEntity(EntityType::MissileProj, position_, glm::vec3(3, 3, 3));
    atk->SetForward(glm::normalize(target_->GetPosition() - GetPosition()));
    atk->SetType(EntityType::EnemyProj);
    atk->SetOrientation(GetOrientation());

    atk->SetSpawnPos(position_);

    atk->SetBlending(true);
    atk->RemoveChildAt(0);

    game_->AddNode(atk);
}
} // namespace game
