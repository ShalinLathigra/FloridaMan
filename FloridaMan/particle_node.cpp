#include "particle_node.h"
#include <iostream>

namespace game {

	ParticleNode::ParticleNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap):SceneNode(name, geometry, material, texture, envmap)
	{
		start = glfwGetTime();
		//std::cout << start << std::endl;
		duration = 1.5f;
	}
	ParticleNode::ParticleNode(const std::string name) :SceneNode(name)
	{
		start = glfwGetTime();
		//std::cout << start << std::endl;
		duration = 1.5f;
	}
	ParticleNode::ParticleNode() :SceneNode()
	{
		start = glfwGetTime();
		//std::cout << start << std::endl;
		duration = 1.5f;
	}

	ParticleNode::~ParticleNode()
	{
	}
            

	void ParticleNode::SetupShader(GLuint program, Camera *camera) 
	{
		SceneNode::SetupShader(program, camera);

		GLint start_var = glGetUniformLocation(program, "start");
		glUniform1f(start_var, start);
		GLint duration_var = glGetUniformLocation(program, "duration");
		glUniform1f(duration_var, duration);
	}

	void ParticleNode::Start(void)
	{
		start = glfwGetTime();
	}
	float ParticleNode::GetStart(void)
	{
		return start;
	}
} // namespace game
