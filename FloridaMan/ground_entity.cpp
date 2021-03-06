#include "ground_entity.h"
#include "game.h"
#include "utilities.h"
#include <iostream>
namespace game
{
GroundEntity::GroundEntity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap)
    : TurretNode(name, geometry, material, texture, envmap)
{
    patrol_angm_ = glm::angleAxis(glm::pi<float>() / 256.0f, glm::vec3(0.0, 1.0, 0.0));
    chase_angm_ = glm::angleAxis(glm::pi<float>() / 128.0f, glm::vec3(0.0, 1.0, 0.0));
    max_idle_timer_ = 3.0f;
    idle_timer_ = max_idle_timer_;

    chase_radius_ = 350.0f;
    chase_angle_ = 0.7f;

    attack_radius_ = 280.0f;
    attack_angle_ = 0.9f;
    max_num_attacks_ = 4;
    num_attacks_ = max_num_attacks_;
    max_attack_timer_ = 4.0f;
    attack_timer_ = 1.0f;
    ammo_ = 100;

    acc_ = 2.5f; // Acceleration Rate
    dec_ = 4.5f; // Deceleration Rate

    vel_ = 0.0f; // Rate of position change
    max_vel_ = 50.0f + utilities::RandPercent() * 90.0f; // Rate of position change
}
GroundEntity::~GroundEntity()
{
}

void GroundEntity::Update(float deltaTime)
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
            GroundEntity::Patrol(deltaTime);
            break;
        case (State::Chase):
            GroundEntity::Chase(deltaTime);
            break;
        case (State::Attack):
            GroundEntity::Attack(deltaTime);
            break;
        case (State::Die):
            TurretNode::Die(deltaTime);
            break;
    }
}

void GroundEntity::Patrol(float deltaTime)
{
    Rotate(patrol_angm_);

    if (vel_ < max_vel_ * 2.0f / 3.0f)
    {
        vel_ = glm::min(vel_ + acc_, max_vel_);
    }
    Translate(GetForward() * vel_ * deltaTime);

    glm::vec3 to_target = target_->GetPosition() - position_;
    float dist_to_target = glm::length(to_target);

    if (dist_to_target < chase_radius_)
    {
        float dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward()));
        if (dot > chase_angle_)
        {
            state_ = State::Chase;
        }
    }
}
void GroundEntity::Chase(float deltaTime)
{
    glm::vec3 to_target = target_->GetPosition() - position_;
    glm::vec3 right = glm::cross(GetForward(), GetUp());

    float dist_to_target = glm::length(to_target);
    float forward_dot = glm::dot(glm::normalize(to_target), glm::normalize(GetForward())); // Near 0 = Turn Hard. Near 1 = Don't Turn. In between = Turn Soft.
    float right_dot = glm::dot(glm::normalize(to_target), glm::normalize(right)); // > 0 = Go Right, < 0 = Go Left

    //Determine Turning
    glm::quat rot;

    if (forward_dot < 0.5)
    {
        rot = chase_angm_;
    }
    else if (forward_dot < 0.95)
    {
        rot = patrol_angm_;
    }

    if (right_dot > 0.0f)
    {
        rot = glm::inverse(rot);
    }

    if (dist_to_target < attack_radius_ * 4.0f / 5.0f)
    {
        vel_ = glm::max(vel_ - dec_, 0.0f);
        state_ = State::Attack;
        attack_timer_ = max_attack_timer_ / 2.0f;
    }
    else
    {
        if (dist_to_target > chase_radius_)
        {
            state_ = State::Patrol;
        }
        else if (dist_to_target < chase_radius_)
        {
            vel_ = glm::min(vel_ + acc_, max_vel_);
        }
        Rotate(rot);
    }

    vel_ = utilities::Clamp(vel_, -max_vel_, max_vel_);

    Translate(GetForward() * vel_ * deltaTime);
}

void GroundEntity::Attack(float deltaTime)
{
    glm::vec3 to_target = target_->GetPosition() - position_;
    float dist_to_target = glm::length(to_target);

    if (dist_to_target > attack_radius_)
    {
        state_ = State::Chase;
    }
    else
    {
        if (attack_timer_ <= 0.0f)
        {
            InstantiateAttack();
            num_attacks_--;
            ammo_--;
            if (num_attacks_ > 0)
            {
                attack_timer_ = max_attack_timer_ / ((float)max_num_attacks_ * 2.0f);
            }
            else
            {
                attack_timer_ = max_attack_timer_;
                num_attacks_ = max_num_attacks_;
            }
        }
        else
        {
            attack_timer_ = glm::max(attack_timer_ - deltaTime, 0.0f);
        }
    }
}
} // namespace game