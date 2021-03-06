#ifndef GROUND_Entity_H_
#define GROUND_Entity_H_

#include "turret_node.h"
//Basic Ground Entity Type
namespace game
{
class GroundEntity : public TurretNode
{
public:
    GroundEntity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
    ~GroundEntity();

    // Update geometry configuration
    void Update(float deltaTime);
    void Patrol(float deltaTime);
    void Chase(float deltaTime);
    void Attack(float deltaTime);

protected:
    // Angular momentum of asteroid
    float chase_radius_;
    float chase_angle_;

    float attack_radius_;
    float attack_angle_;
    int num_attacks_;
    int max_num_attacks_;
    int ammo_;

    float acc_; // Acceleration Rate
    float dec_; // Deceleration Rate

    float vel_; // Rate of position change
    float max_vel_; // Rate of position change

    //Need to handle acceleration, store velocity
};
} // namespace game
#endif // GROUND_Entity_H_