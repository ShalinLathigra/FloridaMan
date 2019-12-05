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
	
	//std::cout << "pos.x = " << position_.x;
	//std::cout << " pos.y = " << position_.y;
	//std::cout << " pos.z = " << position_.z <<"\n";
	glm::vec3 orientation = orientation_ * forward_;
	std::cout << "ori.x = " << orientation.x;
	std::cout << " ori.y = " << orientation.y;
	std::cout << " ori.z = " << orientation.z << "\n";
	//std::cout << " ori.w = " << orientation_.w << "\n";
	
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
	if (m_pPlayer->GetPosition().y > 800 && trans.y > 0)
	{
		trans.y = 0;
	}

	//if (m_pPlayer->CheckCollision(m_pGroundPlane))
	std::vector<SceneNode*> childNodes = m_pPlayer->GetChildren();
	for (int i = 0; i < childNodes.size(); i++)
	{
		std::vector<SceneNode*> grandchildNodes = childNodes[i]->GetChildren();
		childNodes.insert(childNodes.end(), grandchildNodes.begin(), grandchildNodes.end());
		if ((childNodes[i]->GetWorldPosition().y - (childNodes[i]->GetScale().y/2.0)) < 0.0 && trans.y < 0)
		{
			//trans.y = 0;
			
			glm::vec3 t, v2, v1, v3,v4, v5, v6, v7, v8;
			v1 = orientation_ * forward_;
			v2 = v1;
			v2.y = -v1.y;
			/*
			glm::normalize(v1);
			glm::normalize(v2);
			v3 = glm::vec3(v1.x, 0.0, 0.0);
			v4 = glm::vec3(0.0, v1.y, 0.0);
			v5 = glm::vec3(0.0, 0.0, v1.z);
			v6 = glm::vec3(v2.x, 0.0, 0.0);
			v7 = glm::vec3(0.0, v2.y, 0.0);
			v8 = glm::vec3(0.0, 0.0, v2.z);
			

			//acos((v1 DOT v2)/(|v1|*|v2|))
			float angle = acos(glm::dot(v3, v6) / (v3.length() * v6.length()));
			Pitch(angle); // X axis
			angle = acos(glm::dot(v4, v7) / (v4.length() * v7.length()));
			Yaw(-angle); // Y axis
			angle = acos(glm::dot(v5, v8) / (v5.length() * v8.length()));
			Roll(-angle); // Z axis
			//Yaw(glm::pi<float>()/2);
			return;
			*/

			t = glm::cross(v1, v2);
			//glm::normalize(t);
			//glm::normalize(v1);
			//glm::normalize(v2); 
			glm::quat rot;
			rot.x = t.x;
			rot.y = t.y;
			rot.z = t.z;
			rot.w = sqrt((v1.length() ^ 2) * (v2.length() ^ 2)) + glm::dot(v1, v2);
			//glm::normalize(rot);
			Rotate(rot);
			return;
			//skip ahead a couple frames to avoid 'bouncing' more than once
			for (int i = 0; i < 5; i++)
			{
				position_ += trans;
				m_skybox->Translate(trans);
				m_pPlayer->Translate(trans);
			}
			/*
			return;
			*/
		}

	}

	//Note to self, add collision with ground here, and add check for collision with player in each different node types update function, if collision, set the colliding node in the player to be that node: ground, entity, entitystructure, air, turret
    position_ += trans;
	m_skybox->Translate(trans);
    m_pPlayer->Translate(trans);
	//std::cout << "Player's Y position: " << m_pPlayer->GetPosition().y << "\n";
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
