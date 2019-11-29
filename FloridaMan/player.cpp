#include "player.h"
#include "resource_manager.h"

namespace game
{
	Player::Player() :SceneNode("Player")
	{}

	void Player::PlayerInit(ResourceManager *resMan)
	{
		m_pResMan = resMan;
		Resource *geometry = m_pResMan->GetResource("CubeMesh");
		if (geometry->GetType() == PointSet) {
			mode_ = GL_POINTS;
		}
		else if (geometry->GetType() == Mesh) {
			mode_ = GL_TRIANGLES;
		}
		
		Resource *material = m_pResMan->GetResource("MetalMaterial");
		array_buffer_ = geometry->GetArrayBuffer();
		element_array_buffer_ = geometry->GetElementArrayBuffer();
		size_ = geometry->GetSize();
		material_ = material->GetResource();
		texture_ = 0;
		envmap_ = 0;
		m_isVisible = true;
		blending_ = false;
		skybox_ = false;
		scale_ = glm::vec3(1.0, 1.0, 1.0);
		Translate(glm::vec3(0.0, 0.0, 0));
	}
	void Player::Draw(Camera *cam)
	{
		SceneNode::Draw(cam);
	}
}