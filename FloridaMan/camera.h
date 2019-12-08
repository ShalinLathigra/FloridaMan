#ifndef CAMERA_H_
#define CAMERA_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
//#include "scene_node.h"

namespace game
{
class Game;
class Player;
class SceneNode;
class ResourceManager;
// Abstraction of a camera
class Camera
{
public:
    Camera(void);
    ~Camera();

    // Get global camera attributes
    glm::vec3 GetPosition(void) const;
    glm::quat GetOrientation(void) const;

    // Set global camera attributes
    void SetPosition(glm::vec3 position);
    void SetOrientation(glm::quat orientation);

    // Perform global transformations of camera
    void Translate(glm::vec3 trans);
    void Rotate(glm::quat rot);
    Player *GetPlayer();

    // Get relative attributes of camera
    glm::vec3 GetForward(void) const;
    glm::vec3 GetSide(void) const;
    glm::vec3 GetUp(void) const;

    // Perform relative transformations of camera
    void Pitch(float angle, bool rotatePlayer = true);
    void Yaw(float angle, bool rotatePlayer = true);
    void Roll(float angle);

    void move(float speed);
    void SetGame(Game *pGame);

    void SetSkyBox(SceneNode *skybox);

    // Set the view from camera parameters: initial position of camera,
    // point looking at, and up vector
    // Resets the current orientation and position of the camera
    void SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up);
    // Set projection from frustum parameters: field-of-view,
    // near and far planes, and width and height of viewport
    void SetProjection(GLfloat fov, GLfloat near, GLfloat far, GLfloat w, GLfloat h);
    // Set all camera-related variables in shader program
    void SetupShader(GLuint program);

    // Return view matrix
    glm::mat4 GetCurrentViewMatrix(void);
    void TogglePOV();
    void InitPlayer(ResourceManager *resMan);
    void SetGround(SceneNode *pGround);

private:
    glm::vec3 CalculateCollisionNormal(SceneNode *pNode, SceneNode *pNodeHit);
    Game *m_pGame;
    // Skybox
    SceneNode *m_pGroundPlane;
    SceneNode *m_skybox;
    bool m_isFirstPerson;
    glm::vec3 position_; // Position of camera
    glm::quat orientation_; // Orientation of camera
    glm::vec3 forward_; // Initial forward vector
    glm::vec3 side_; // Initial side vector
    glm::mat4 view_matrix_; // View matrix
    glm::mat4 projection_matrix_; // Projection matrix

    Player *m_pPlayer;
    // Create view matrix from current camera parameters
    void SetupViewMatrix(void);

}; // class Camera

} // namespace game

#endif // CAMERA_H_
