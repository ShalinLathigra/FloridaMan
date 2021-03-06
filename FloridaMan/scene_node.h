#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "camera.h"
#include "resource.h"

namespace game
{
class Game;
// Class that manages one object in a scene
class SceneNode
{
public:
    SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *envmap = NULL);
    SceneNode(const std::string name);
    SceneNode();
    // Destructor
    ~SceneNode();

    // Get name of node
    const std::string GetName(void) const;

    // Get node attributes
    glm::vec3 GetPosition(void) const;
    glm::quat GetOrientation(void) const;
    glm::vec3 GetScale(void) const;
    bool GetBlending(void) const;

    // Set node attributes
    void SetPosition(glm::vec3 position);
    void SetOrientation(glm::quat orientation);
    void SetScale(glm::vec3 scale);
    void SetBlending(bool blending);
    void SetSkybox(bool skybox);

    // Perform transformations on node
    void Translate(glm::vec3 trans);
    void Rotate(glm::quat rot);
    void Scale(glm::vec3 scale);

    // Draw the node according to scene parameters in 'camera'
    // variable
    virtual void Draw(Camera *camera);

    // Update the node
    virtual void Update(float deltaTime);
    bool CheckCollision(SceneNode *pNode, SceneNode **collidingPart = nullptr);
    bool CheckSphereCollision(SceneNode *pNode, float dist);

    // OpenGL variables
    GLenum GetMode(void) const;
    GLuint GetArrayBuffer(void) const;
    GLuint GetElementArrayBuffer(void) const;
    GLsizei GetSize(void) const;
    GLuint GetMaterial(void) const;

    glm::vec3 GetWorldPosition();
    glm::quat GetWorldOrientation();
    bool checkIfDestroy();
    SceneNode *GetParent();
    void SetParent(SceneNode *pParent);
    std::vector<SceneNode *> GetChildren();
    void AddChild(SceneNode *pChildNode);
    SceneNode *FindChild(std::string nodeName);

    SceneNode *RemoveChildAt(int index);

    inline void SetType(int type) { type_ = type; }
    inline int GetType(void) { return type_; }
    void SetGame(Game *g);

    void SetUpdated(bool u);
    bool GetUpdated(void);
    void setDestroyFlag(bool toggle);

protected:
    std::string name_; // Name of the scene node
    GLuint array_buffer_; // References to geometry: vertex and array buffers
    GLuint element_array_buffer_;
    GLenum mode_; // Type of geometry
    GLsizei size_; // Number of primitives in geometry
    GLuint material_; // Reference to shader program
    GLuint texture_; // Reference to texture resource
    GLuint envmap_; // Reference to environment map
    glm::vec3 position_; // Position of node
    glm::quat orientation_; // Orientation of node
    glm::vec3 scale_; // Scale of node
    bool skybox_; //Is skybox or not
    bool blending_; // Draw with blending or not
    bool m_isVisible; //Is the node visible or not
    bool set_toDestroy = false; //Check if this node needs to be removed
    SceneNode *m_pParentNode;
    std::vector<SceneNode *> m_childNodes;
    // Set matrices that transform the node in a shader program
    virtual void SetupShader(GLuint program, Camera *camera);
    glm::mat4 GetParentTransform();

    int type_;
    Game *game_;
    bool updated_;
}; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
