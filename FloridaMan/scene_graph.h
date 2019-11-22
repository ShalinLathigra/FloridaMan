#ifndef SCENE_GRAPH_H_
#define SCENE_GRAPH_H_

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene_node.h"
#include "laser_node.h"
#include "air_enemy.h"
#include "resource.h"
#include "mine.h"
#include "camera.h"

// Size of the texture that we will draw
#define FRAME_BUFFER_WIDTH 1024
#define FRAME_BUFFER_HEIGHT 768

namespace game {

    // Class that manages all the objects in a scene
    class SceneGraph {

        private:
            // Background color
            glm::vec3 background_color_;

            // Scene nodes to render
			SceneNode *m_pRootNode;

            // Frame buffer for drawing to texture
            GLuint frame_buffer_;
            // Quad vertex array for drawing from texture
            GLuint quad_array_buffer_;
            // Render targets
            GLuint texture_;
            GLuint depth_buffer_;

        public:
            // Constructor and destructor
            SceneGraph(void);
            ~SceneGraph();

            // Background color
            void SetBackgroundColor(glm::vec3 color);
            glm::vec3 GetBackgroundColor(void) const;
            
            // Create a scene node from the specified resources
            SceneNode *CreateNode(int type, std::string node_name, Resource *geometry, Resource *material, Resource *texture = NULL, Resource *envmap = NULL);
            // Add an already-created node to the root of the tree
            void AddNode(SceneNode *node);
            // Find a scene node with a specific name
            SceneNode *GetNode(std::string node_name) const;
            

            // Draw the entire scene
            void Draw(Camera *camera);

            // Update entire scene
            void Update(float deltaTime);

            // Drawing from/to a texture
            // Setup the texture
            void SetupDrawToTexture(void);
            // Draw the scene into a texture
            void DrawToTexture(Camera *camera);
            // Process and draw the texture on the screen
            void DisplayTexture(GLuint program);
            // Save texture to a file in ppm format
            void SaveTexture(char *filename);

    }; // class SceneGraph

} // namespace game

#endif // SCENE_GRAPH_H_
