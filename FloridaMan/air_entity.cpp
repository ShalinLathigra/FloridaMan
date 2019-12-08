#include "air_entity.h"
#include "game.h"
#include "utilities.h"
#include <iostream>
namespace game
{
AirEntity::AirEntity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap)
    : GroundEntity(name, geometry, material, texture, envmap)
{
    chase_radius_ = 230.0f;
    chase_angm_ = glm::angleAxis(glm::pi<float>() / 256.0f, glm::vec3(0, 1, 0));

    attack_angle_ = 0.99f;
    attack_radius_ = 100.0f;

    mid_y_ = 10.f;
    y_offset_[0] = -5.0f;
    y_offset_[1] = 0.0f;
    y_offset_[2] = 5.0f;
    off_index_ = 0;
    off_step_ = 1;
    desired_y_ = mid_y_ + y_offset_[off_index_];

	ammo_ = 4 + (int)(utilities::RandPercent() * 6.0f);
	max_num_attacks_ = 2;
	num_attacks_ = max_num_attacks_;
	max_attack_timer_ = 3.0f;
	attack_timer_ = max_attack_timer_ / 6.0f;



    off_timer_ = 0.0f;
    max_off_timer_ = 2.5f;

    y_speed_ = 0.75f;
	hover_speed_ = 27.0f + utilities::RandPercent() * 54.0f;
	to_target_ = glm::vec3(0);
}
AirEntity::~AirEntity()
{
}

void AirEntity::Update(float deltaTime)
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
            AirEntity::Patrol(deltaTime);
            break;
        case (State::Chase):
            AirEntity::Chase(deltaTime);
            break;
        case (State::Attack):
            AirEntity::Attack(deltaTime);
            break;
        case (State::Die):
            TurretNode::Die(deltaTime);
            break;
    }
}

void AirEntity::Patrol(float deltaTime)
{
    //X-z movement covered here. I need to re-work ywards movement

    GroundEntity::Patrol(deltaTime);

    AssessYOffset(deltaTime);
    MaintainY(mid_y_, deltaTime);

    SetState();
}
void AirEntity::Chase(float deltaTime)
{
    GroundEntity::Chase(deltaTime);

    AssessYOffset(deltaTime);
    MaintainY(target_->GetPosition().y, deltaTime);

    SetState();
}

void AirEntity::Attack(float deltaTime)
{
	if (ammo_ > 0)
	{
		glm::vec3 right = glm::normalize(glm::cross(GetForward(), GetUp()));
		Translate(hover_speed_ * right * deltaTime);
		TurretNode::Chase(deltaTime);

		AssessYOffset(deltaTime);
		MaintainY(target_->GetPosition().y, deltaTime);


		to_target_ = target_->GetPosition() - position_;
		float dist_to_target = glm::length(to_target_);

		to_target_ = glm::normalize(to_target_);

		if (dist_to_target > attack_radius_)
		{
			state_ = State::Chase;
		}
		else
		{
			GroundEntity::Attack(deltaTime);
		}
		SetState();
	}
	else
	{
		if (attack_timer_ == 0.0f)
		{
			to_target_ = glm::normalize(target_->GetPosition() - position_);
			attack_timer_ = max_attack_timer_;
		}
		else
		{
			attack_timer_ = glm::max(attack_timer_ - deltaTime, 0.0f);
		}

		Translate(hover_speed_ * 5.0f * to_target_ * deltaTime);
		if (CheckCollision(target_))
		{
			if (target_->GetName().find("Player"))
			{
				((Player *)target_)->TakeDamage(25.0f);
				TakeDamage(100.0f);
			}
			target_->SetType((target_->GetType() + 1) % 4);
		}
	}

}

void AirEntity::SetState()
{
    glm::vec3 to_target = target_->GetPosition() - position_;
    float dist_to_target = glm::length(to_target);
    if (dist_to_target > chase_radius_)
    {
        state_ = State::Patrol;
    }
    else if (dist_to_target < attack_radius_)
    {
        state_ = State::Attack;
    }
    else
    {
        state_ = State::Chase;
    }
}

void AirEntity::MaintainY(float target_y, float deltaTime)
{
    desired_y_ = glm::max(target_y + y_offset_[off_index_], 60.0f);

    float y_vel = desired_y_ - position_.y;

    y_vel = utilities::Clamp(y_vel, -y_speed_, y_speed_);

    if (y_vel * y_vel > y_diff_ * y_diff_)
    {
        position_.y += y_speed_ * y_vel * deltaTime;
    }
}

void AirEntity::AssessYOffset(float deltaTime)
{
    if (position_.y != desired_y_)
    {
        float y_vel_ = y_speed_ * (desired_y_ - position_.y);

        if (abs(y_vel_) < .025f)
        {
            position_.y = desired_y_;
        }
        else
        {
            position_.y += y_vel_ * deltaTime;
        }
    }
    else
    {
        //Logic for changing the desired level
        off_timer_ = glm::max(off_timer_ - deltaTime, 0.0f);
        if (off_timer_ == 0.0f)
        {
            off_timer_ = max_off_timer_;
            off_index_ += off_step_;

            if (off_index_ == 0)
            {
                off_step_ *= -1;
            }
            else if (off_index_ == 2)
            {
                off_step_ *= -1;
            }
        }
    }
}
} // namespace game