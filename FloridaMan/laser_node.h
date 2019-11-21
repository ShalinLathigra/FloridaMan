
#ifndef LASER_H_
#define LASER_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"

#include <algorithm>    // std::max

namespace game
{
	class LaserNode :
		public SceneNode
	{
	public:
		LaserNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap);
		~LaserNode();

		void Draw(Camera *camera) override;
		void Update(float deltaTime);
		void AdvanceTimers(float deltaTime);

		void Fire(void);
		void StartCooldown(void);
		bool IsActive(void);

		void turnOff(void);

		float GetWidth(void) const;
		glm::vec3 GetForward(void) const;
		void SetJoint(glm::vec3);

		glm::mat4 CalculateMatrix(glm::mat4 p);

		void setParent(Camera *camera);
	private:
		bool active_;

		//Replace this with the player node to transform off of
		Camera *parent_node_;

		float width_;
		glm::vec3 forward_;	//initial up

		glm::vec3 joint_;
		glm::quat orbit_amount_;

		float max_duration_timer_;
		float max_cd_timer_;

		float duration_timer_;
		float cd_timer_;
	};
}

#endif // ASTEROID_H_
