#ifndef PLAYER_H_
#define PLAYER_H_
#include "scene_node.h"
#include "utilities.h"

#define MAX_SHURIKEN_TIMER 0.25f
#define MAX_BOMB_TIMER 2.5f
#define MAX_MINE_TIMER 5.0f

namespace game
{
class ResourceManager;
class Player : public SceneNode
{
public:
    Player();
    void PlayerInit(ResourceManager *resMan);
    virtual void Draw(Camera *camera) override;
    virtual void Update(float deltaTime) override;

    void Fire(int entityType);
	void TakeDamage(float amount);
	void SetCamera(Camera *cam);
	void Accellerate(float speed);
private:
	float m_speed;
	Camera *m_pCamera;
    ResourceManager *m_pResMan;
	glm::vec3 m_forward;
    float m_ShurikenTimer;
    float m_BombTimer;
    float m_MineTimer;
	float m_HP;
};
} // namespace game

#endif