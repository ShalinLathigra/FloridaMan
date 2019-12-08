#ifndef AIR_Entity_H_
#define AIR_Entity_H_

#include "ground_entity.h"

//Basic Ground Entity Type
namespace game
{
class AirEntity : public GroundEntity
{
public:
    AirEntity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
    ~AirEntity();

    // Update geometry configuration
    void Update(float deltaTime);
    void Patrol(float deltaTime);
    void Chase(float deltaTime);
    void Attack(float deltaTime);

    void SetState();
    void MaintainY(float target_y, float deltaTime);
    void AssessYOffset(float deltaTime);

protected:
    float mid_y_;
    int y_offset_[3];
    int off_index_;
    int off_step_;
    float desired_y_;

    float off_timer_;
    float max_off_timer_;

    float y_diff_;
    float y_speed_;

    float hover_speed_;

    glm::vec3 to_target_;

    //Need to handle acceleration, store velocity
};
} // namespace game
#endif // AIR_Entity_H_