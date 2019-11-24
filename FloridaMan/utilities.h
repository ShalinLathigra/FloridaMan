#ifndef UTILITIES_H
#define UTILITIES_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

// This namespace contains a number of useful functions for a variety of different purposes, including creating 3D models, and random number generation.
namespace game
{
	enum EntityType { Turret, Ground, Air, MineProj, BombProj, ShurikenProj, Particle, TurretSpawn, GroundSpawn, AirSpawn, Default };
}
namespace utilities
{
	inline glm::vec3 RotateVecByQuat(glm::vec3 vec, glm::quat q) {
		// Extract the vector part of the quaternion
		glm::vec3 u = glm::vec3(q.x, q.y, q.z);

		// Extract the scalar part of the quaternion
		float s = q.w;

		// Do the math
		glm::vec3 newVec = 2.0f * glm::dot(u, vec) * u
			+ (s*s - glm::dot(u, u)) * vec
			+ 2.0f * s * glm::cross(u, vec);
		return newVec;
	}

	//Returns either 1 or -1, randomly
	inline int RandSign()
	{
		return rand() % 2 == 0 ? -1 : 1;
	}

	//Returns a random float between 0.00 and 1.0
	inline float RandPercent()
	{
		return (((float)(rand() % 100) + 1.0f) / 100.0f);
	}

	inline glm::quat RandQuat(glm::vec3 axis)
	{
		return glm::angleAxis((((float)(rand() % 100) + 1.0f) / 100.0f)*glm::pi<float>()*2.0f, axis);
	}
	
	inline float Clamp(float in, float low, float high)
	{
		return (in < low) ? low : (in > high) ? high : in;
	}
}
#endif