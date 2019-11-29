#ifndef PLAYER_H_
#define PLAYER_H_
#include "scene_node.h"


namespace game
{
	class ResourceManager;
	class Player : public SceneNode
	{
	public:
		Player();
		void PlayerInit(ResourceManager *resMan);
		virtual void Draw(Camera *camera) override;
	private:
		ResourceManager *m_pResMan;
	};
}

#endif