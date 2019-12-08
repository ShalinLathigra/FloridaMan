#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"
#include "player.h"
#include "scene_node.h"
#include "scene_graph.h"
#include "game.h"

namespace game
{
Camera::Camera(void)
{
    m_isFirstPerson = true;
}

Camera::~Camera()
{
}

glm::vec3 Camera::GetPosition(void) const
{
    return position_;
}

void Camera::move(float iAmSpeed)
{
	Translate(iAmSpeed * (orientation_ * forward_));
}

glm::quat Camera::GetOrientation(void) const
{
    return orientation_;
}

void Camera::SetSkyBox(SceneNode *skybox)
{
	m_skybox = skybox;
}

void Camera::SetPosition(glm::vec3 position)
{
    //m_pPlayer->SetPosition(position);
    position_ = position;
}

void Camera::TogglePOV()
{
    m_isFirstPerson = !m_isFirstPerson;
}

void Camera::SetOrientation(glm::quat orientation)
{
    orientation_ = orientation;
    m_pPlayer->SetOrientation(orientation_);
}

void Camera::SetGame(Game *pGame)
{
	m_pGame = pGame;
}

void Camera::SetGround(SceneNode *pGround)
{
	m_pGroundPlane = pGround;
}

void Camera::Translate(glm::vec3 trans)
{
	if (m_pPlayer->GetPosition().y > 700 && trans.y > 0)
	{
		trans.y = 0;
		std::cout << "You're leaving the raid area!! You can't abandon the aliens!\n";
	}

	if (m_pPlayer->GetPosition().x > 1250 && trans.x > 0)
	{
		trans.x = 0;
		std::cout << "You're leaving the raid area!! You can't abandon the aliens!\n";
	}
	if (m_pPlayer->GetPosition().x < -1250 && trans.x < 0)
	{
		trans.x = 0;
		std::cout << "You're leaving the raid area!! You can't abandon the aliens!\n";
	}
	if (m_pPlayer->GetPosition().z > 1250 && trans.z > 0)
	{
		trans.z = 0;
		std::cout << "You're leaving the raid area!! You can't abandon the aliens!\n";
	}
	if (m_pPlayer->GetPosition().z < -1250 && trans.z < 0)
	{
		trans.z = 0;
		std::cout << "You're leaving the raid area!! You can't abandon the aliens!\n";
	}

	std::vector<SceneNode*> childNodes = m_pPlayer->GetChildren();
	for (int i = 0; i < childNodes.size(); i++)
	{
		std::vector<SceneNode*> grandchildNodes = childNodes[i]->GetChildren();
		childNodes.insert(childNodes.end(), grandchildNodes.begin(), grandchildNodes.end());
		if ((childNodes[i]->GetWorldPosition().y - (childNodes[i]->GetScale().y/2.0)) < 0.0 && trans.y < 0)
		{
			glm::vec3 t, v2, v1;
			v1 = orientation_ * forward_;
			v2 = v1;
			v2.y = -v1.y;

			t = glm::cross(v1, v2);

			//Construct the rotation
			glm::quat rot;
			rot.x = t.x;
			rot.y = t.y;
			rot.z = t.z;
			rot.w = sqrt((v1.length() ^ 2) * (v2.length() ^ 2)) + glm::dot(v1, v2);

			//Slow down and damage the player after a collision
			m_pPlayer->Accelerate(m_pPlayer->m_speed * -0.05);
			m_pPlayer->m_HP -= 5;
			Rotate(rot);
			return;
		}

	}

	if (m_pPlayer->m_pCollisionEntity &&!m_pPlayer->m_Ramming)
	{
		
		glm::vec3 collisionDirection = CalculateCollisionNormal(m_pPlayer->m_pCollisionEntity, m_pPlayer->m_pNodeHit) * -1.0f;
		m_pPlayer->m_pCollisionEntity = nullptr;
		glm::normalize(collisionDirection);
		glm::vec3 t, v2, v1;
		v1 = orientation_ * forward_;
		glm::normalize(v1);
		v2 = (collisionDirection + v1);
		glm::normalize(v2);
		t = glm::cross(v1, v2);
		glm::quat rot;
		rot.x = t.x;
		rot.y = t.y;
		rot.z = t.z;
		rot.w = sqrt((v1.length() ^ 2) * (v2.length() ^ 2)) + glm::dot(v1, v2);

		Rotate(rot);
		
		//Slow down and damage the player after a collision
		m_pPlayer->Accelerate(m_pPlayer->m_speed * -0.05);
		m_pPlayer->m_HP -= 15;
		//Add an offset so that the player isn't partially inside the block
		position_ -= glm::normalize(orientation_ * forward_) * 3.0f;
		m_skybox->SetPosition(position_);
		m_pPlayer->SetPosition(position_);
		return;

	}
	else if (m_pPlayer->m_pCollisionEntity)
	{
		Entity *entity = dynamic_cast<Entity*>(m_pPlayer->m_pCollisionEntity);
		if (entity)
		{
			entity->SetState(State::Die);
		}
	}

	 position_ += trans;
	m_skybox->Translate(trans);
    m_pPlayer->Translate(trans);
}

glm::vec3 Camera::CalculateCollisionNormal(SceneNode *pNode, SceneNode *pNodeHit)
{
	glm::vec3 normal;
	glm::vec3 nodePos = pNode->GetWorldPosition();
	glm::vec3 playerPos = pNodeHit->GetWorldPosition();
	glm::vec3 dist = playerPos - nodePos;
	if (abs(dist.x) + pNode->GetScale().x  + pNodeHit->GetScale().x < abs(dist.y) + pNode->GetScale().y + pNodeHit->GetScale().y && abs(dist.x) + pNode->GetScale().x + pNodeHit->GetScale().x < abs(dist.z) + pNode->GetScale().z + pNodeHit->GetScale().z)
	{
		normal = glm::vec3(1, 0, 0);
		if (dist.x > 0)
		{
			normal.x = -1;
		}
	}
	else if (abs(dist.y) + pNode->GetScale().y + pNodeHit->GetScale().y < abs(dist.x) + pNode->GetScale().x + pNodeHit->GetScale().x && abs(dist.y) + pNode->GetScale().y + pNodeHit->GetScale().y < abs(dist.z) + pNode->GetScale().z + pNodeHit->GetScale().z)
	{
		normal = glm::vec3(0, 1, 0);
		if (dist.y > 0)
		{
			normal.y = -1;
		}
	}
	else if (abs(dist.z) + pNode->GetScale().z + pNodeHit->GetScale().z < abs(dist.x) + pNode->GetScale().x + pNodeHit->GetScale().x && abs(dist.z) + pNode->GetScale().z + pNodeHit->GetScale().z < abs(dist.y) + pNode->GetScale().y + pNodeHit->GetScale().y)
	{
		normal = glm::vec3(0, 0, 1);
		if (dist.z > 0)
		{
			normal.z = -1;
		}
	}
	normal = utilities::RotateVecByQuat(normal, pNode->GetWorldOrientation());
	return normal;
}

void Camera::Rotate(glm::quat rot)
{
    orientation_ = rot * orientation_;
    orientation_ = glm::normalize(orientation_);
    m_pPlayer->SetOrientation(orientation_);
}

glm::vec3 Camera::GetForward(void) const
{
    glm::vec3 current_forward = orientation_ * forward_;
    return -current_forward; // Return -forward since the camera coordinate system points in the opposite direction
}

glm::vec3 Camera::GetSide(void) const
{
    glm::vec3 current_side = orientation_ * side_;
    return current_side;
}

glm::vec3 Camera::GetUp(void) const
{
    glm::vec3 current_forward = orientation_ * forward_;
    glm::vec3 current_side = orientation_ * side_;
    glm::vec3 current_up = glm::cross(current_forward, current_side);
    current_up = glm::normalize(current_up);
    return current_up;
}

void Camera::Pitch(float angle, bool rotatePlayer)
{
    glm::quat rotation = glm::angleAxis(angle, GetSide());
    orientation_ = rotation * orientation_;
    orientation_ = glm::normalize(orientation_);
	if (rotatePlayer)
	{
		m_pPlayer->SetOrientation(orientation_);
	}
}

void Camera::Yaw(float angle, bool rotatePlayer)
{
    glm::quat rotation = glm::angleAxis(angle, GetUp());
    orientation_ = rotation * orientation_;
    orientation_ = glm::normalize(orientation_);
	if (rotatePlayer)
	{
		m_pPlayer->SetOrientation(orientation_);
	}
}

void Camera::Roll(float angle)
{
    glm::quat rotation = glm::angleAxis(angle, GetForward());
    orientation_ = rotation * orientation_;
    orientation_ = glm::normalize(orientation_);
    m_pPlayer->SetOrientation(orientation_);
}

void Camera::SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up)
{
    // Store initial forward and side vectors
    // See slide in "Camera control" for details
    forward_ = look_at - position;
    forward_ = -glm::normalize(forward_);
    side_ = glm::cross(up, forward_);
    side_ = glm::normalize(side_);

    // Reset orientation and position of camera
    position_ = position;
    orientation_ = glm::quat();
}

void Camera::SetProjection(GLfloat fov, GLfloat near, GLfloat far, GLfloat w, GLfloat h)
{
    // Set projection based on field-of-view
    float top = tan((fov / 2.0) * (glm::pi<float>() / 180.0)) * near;
    float right = top * w / h;
    projection_matrix_ = glm::frustum(-right, right, -top, top, near, far);
}

void Camera::SetupShader(GLuint program)
{
    // Update view matrix
    SetupViewMatrix();

    // Set view matrix in shader
    GLint view_mat = glGetUniformLocation(program, "view_mat");
    glUniformMatrix4fv(view_mat, 1, GL_FALSE, glm::value_ptr(view_matrix_));

    // Set projection matrix in shader
    GLint projection_mat = glGetUniformLocation(program, "projection_mat");
    glUniformMatrix4fv(projection_mat, 1, GL_FALSE, glm::value_ptr(projection_matrix_));

    // Set camera position in world coordinates
    GLint position_vec = glGetUniformLocation(program, "camera_pos");
    glUniform3fv(position_vec, 1, glm::value_ptr(position_));
}

glm::mat4 Camera::GetCurrentViewMatrix(void)
{
    return view_matrix_;
}

void Camera::InitPlayer(ResourceManager *resMan)
{
    m_pPlayer = new Player();
    m_pPlayer->PlayerInit(resMan);
}

Player *Camera::GetPlayer()
{
    return m_pPlayer;
}

void Camera::SetupViewMatrix(void)
{
    //view_matrix_ = glm::lookAt(position, look_at, up);

	if (!m_isFirstPerson)
	{
		Yaw(-0.15f, false);
		Pitch(-0.25f, false);
	}
    // Get current vectors of coordinate system
    // [side, up, forward]
    // See slide in "Camera control" for details
    glm::vec3 current_forward = orientation_ * forward_;
    glm::vec3 newPosition = m_pPlayer->GetPosition();
    glm::vec3 current_side = orientation_ * side_;
    glm::vec3 current_up = glm::cross(current_forward, current_side);
    current_up = glm::normalize(current_up);
    if (m_isFirstPerson)
    {
        SetPosition(newPosition - current_forward);
    }
    else
    {
        SetPosition(newPosition + current_forward * 10.0f +/* current_up * 2.0f + */current_side * 2.0f);
    }

    // Initialize the view matrix as an identity matrix
    view_matrix_ = glm::mat4(1.0);

    // Copy vectors to matrix
    // Add vectors to rows, not columns of the matrix, so that we get
    // the inverse transformation
    // Note that in glm, the reference for matrix entries is of the form
    // matrix[column][row]
    view_matrix_[0][0] = current_side[0]; // First row
    view_matrix_[1][0] = current_side[1];
    view_matrix_[2][0] = current_side[2];
    view_matrix_[0][1] = current_up[0]; // Second row
    view_matrix_[1][1] = current_up[1];
    view_matrix_[2][1] = current_up[2];
    view_matrix_[0][2] = current_forward[0]; // Third row
    view_matrix_[1][2] = current_forward[1];
    view_matrix_[2][2] = current_forward[2];

    // Create translation to camera position
    glm::mat4 trans = glm::translate(glm::mat4(1.0), -position_);

    // Combine translation and view matrix in proper order
    view_matrix_ *= trans; //glm::mat4_cast(orientation_) * trans;

	if (!m_isFirstPerson)
	{
		//reset the orientation to be equal to the players if necessary
		SetOrientation(m_pPlayer->GetOrientation());
	}
}

} // namespace game
