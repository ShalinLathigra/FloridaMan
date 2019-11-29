#ifndef PLAYER_H_
#define PLAYER_H_
#include "scene_node.h"
#include "utilities.h"
#include "particle_node.h"

#define MAX_SHURIKEN_TIMER 0.25f
#define MAX_BOMB_TIMER 2.5f
#define MAX_MINE_TIMER 5.0f

namespace game
{
class ResourceManager;
class Player : public SceneNode
{
	friend Camera;
public:
    Player();
    void PlayerInit(ResourceManager *resMan);
    virtual void Draw(Camera *camera) override;
    virtual void Update(float deltaTime) override;
	
    void Fire(int entityType);
	void TakeDamage(float amount);
	void SetCamera(Camera *cam);

	void Accelerate(float speed);
	void SetCollisionEntity(SceneNode *collisionEntity);
	void SetPart(ParticleNode *part);
	float GetHP();
private:
	SceneNode *m_pCollisionEntity;
	float m_speed;
	Camera *m_pCamera;
	ResourceManager *m_pResMan;
	ParticleNode m_part;
	glm::vec3 m_forward;
    float m_ShurikenTimer;
    float m_BombTimer;
    float m_MineTimer;
	float m_HP;
};
} // namespace game

#endif