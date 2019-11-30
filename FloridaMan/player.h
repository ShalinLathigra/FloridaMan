#ifndef PLAYER_H_
#define PLAYER_H_
#include "scene_node.h"
#include "utilities.h"

#define MAX_SHURIKEN_TIMER 0.05f
#define MAX_BOMB_TIMER 0.5f
#define MAX_MINE_TIMER 1.0f

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

private:
    ResourceManager *m_pResMan;

    float m_ShurikenTimer;
    float m_BombTimer;
    float m_MineTimer;
};
} // namespace game

#endif