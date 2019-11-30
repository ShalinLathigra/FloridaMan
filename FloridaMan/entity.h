#ifndef Entity_H_
#define Entity_H_

#include "particle_node.h"
#include "scene_node.h"

namespace game
{
enum State
{
    Idle,
    Patrol,
    Chase,
    Attack,
    Die
};

//Entity Superclass
class Entity : public SceneNode
{
public:
    Entity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
    ~Entity();

    void Update(float deltaTime);

    virtual void TakeDamage(float amount);

    State GetState(void);
    Camera *GetTarget(void);
    glm::vec3 GetForward(void);
    glm::vec3 GetUp(void);
    void SetState(State state);
    void SetTarget(Camera *target);
    void SetEndScale(glm::vec3 end_scale);

    bool IsAlive(void);
    void SetDeathEffect(ParticleNode part);

protected:
    State state_;
    Camera *target_;
    glm::vec3 forward_;
    glm::vec3 up_;
    float hp_;
    float max_hp_;

    float idle_timer_;
    float max_idle_timer_;

    glm::vec3 end_scale_;

    ParticleNode death_particles_;
    bool death_part_;
};
} // namespace game
#endif // Entity_H_