#ifndef MINE_H_
#define MINE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <algorithm> // std::max
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "resource.h"
#include "scene_node.h"

namespace game
{
enum MineState
{
    MineIdle,
    MineChase,
    Boom
};

class Mine : public SceneNode
{
public:
    Mine(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
    ~Mine();

    void Update(float deltaTime);

    void Idle(float deltaTime);
    void Chase(float deltaTime);
    void Boom(float deltaTime);

    MineState GetState(void);
    SceneNode *GetTarget(void);
    void SetState(MineState state);
	void SetTarget(SceneNode *target);
	void InitAltMat(ResourceManager* resman_);

	inline float GetBoomRadius(void) { return boom_radius_; }
	inline float GetChaseRadius(void) { return chase_radius_; }
	virtual void Draw(Camera *camera) override;

protected:
	GLuint alt_mat_;
    MineState state_;
    SceneNode *target_;
    glm::vec3 forward_;
    glm::vec3 up_;
    float hp_;
    bool target_set_;
    float vel_;
	float acc_;
    std::vector<SceneNode> enemies;

    float idle_timer_;
    float max_idle_timer_;

    float boom_radius_;
	float chase_radius_;
};
} // namespace game

#endif // MINE_H_
