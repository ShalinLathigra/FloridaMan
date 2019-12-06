#ifndef BOMB_H_
#define BOMB_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <algorithm> // std::max
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"
#include "particle_node.h"

namespace game
{
enum BombState
{
    BombFall,
    BombBoom
};

class Bomb : public SceneNode
{
public:
    Bomb(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
    ~Bomb();

    void Update(float deltaTime);

    void Fall(float deltaTime);
    void Boom(float deltaTime);

    BombState GetState(void);
	void SetState(BombState state);
	void SetDeathPart(ParticleNode part);

protected:
    BombState state_;
	ParticleNode death_part_;
    glm::vec3 forward_;
    glm::vec3 up_;
	glm::vec3 velocity_;
	float grav_;

    float idle_timer_;
    float max_idle_timer_;

    float boom_radius_;
};
} // namespace game

#endif // MINE_H_
