#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "bin/path_config.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 50.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_g(0.0, 0.0, 0.0);
glm::vec3 camera_look_at_g(0.0, 0.0, -10.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;


Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();

    // Set variables
    animating_ = true;
}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
    }
    if (!window_){
        glfwTerminate();
        throw(GameException(std::string("Could not create window")));
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        throw(GameException(std::string("Could not initialize the GLEW library: ")+std::string((const char *) glewGetErrorString(err))));
    }
}


void Game::InitView(void){

    // Set up z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Set up camera
    // Set current view
    camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){

    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){

    // Create a torus
    resman_.CreateTorus("TorusMesh");

    // Use sphere to better analyze the environment map
    //resman_.CreateSphere("TorusMesh");

    // Can also check reflections on a cube
    //std::string cube_filename = std::string(MATERIAL_DIRECTORY) + std::string("/dense_cube.obj");
    //resman_.LoadResource(Mesh, "TorusMesh", cube_filename.c_str());

    // Create a cube for the skybox
    resman_.CreateCube("CubeMesh");

    // Load material to be applied to torus
    std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/envmap");
    resman_.LoadResource(Material, "EnvMapMaterial", filename.c_str());

    // Load cube map to be applied to skybox
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/island/island.tga");
    resman_.LoadResource(CubeMap, "LakeCubeMap", filename.c_str());

    // Load material to be applied to skybox
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/skybox");
    resman_.LoadResource(Material, "SkyboxMaterial", filename.c_str());
}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

	// Create an instance of the torus mesh
	StaticEnemy *torus1 = (StaticEnemy*)CreateEnemy("TorusInstance1", "TorusMesh", "EnvMapMaterial", "", "LakeCubeMap");
	// Scale the instance
	torus1->StaticEnemy::Init();
	torus1->Scale(glm::vec3(1.5, 1.5, 1.5));
	torus1->Translate(glm::vec3(1.0, 1.0, -10.0));
	torus1->SetTarget(&camera_);
	
	StaticEnemy *torus2 = (StaticEnemy*)CreateEnemy("TorusInstance2", "TorusMesh", "EnvMapMaterial", "", "LakeCubeMap");
	// Scale the instance
	torus2->StaticEnemy::Init();
	torus2->Scale(glm::vec3(1.5, 1.5, 1.5));
	torus2->Translate(glm::vec3(1.0, -1.0, -10.0));
	torus2->SetTarget(&camera_);
	
	StaticEnemy *torus3 = (StaticEnemy*)CreateEnemy("TorusInstance3", "TorusMesh", "EnvMapMaterial", "", "LakeCubeMap");
	// Scale the instance
	torus3->StaticEnemy::Init();
	torus3->Scale(glm::vec3(1.5, 1.5, 1.5));
	torus3->Translate(glm::vec3(-1.0, 1.0, -10.0));
	torus3->SetTarget(&camera_);
	
	StaticEnemy *torus4 = (StaticEnemy*)CreateEnemy("TorusInstance4", "TorusMesh", "EnvMapMaterial", "", "LakeCubeMap");
	// Scale the instance
	torus4->StaticEnemy::Init();
	torus4->Scale(glm::vec3(1.5, 1.5, 1.5));
	torus4->Translate(glm::vec3(-1.0, -1.0, -10.0));
	torus4->SetTarget(&camera_);
	
	
    // Create skybox
    skybox_ = CreateInstance("CubeInstance1", "CubeMesh", "SkyboxMaterial", "LakeCubeMap");
    skybox_->Scale(glm::vec3(50.0, 50.0, 50.0));
}


void Game::MainLoop(void){

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        // Animate the scene
        if (animating_){
            static double last_time = 0;
            double current_time = glfwGetTime();
			float deltaTime = current_time - last_time;
            if (deltaTime > 0.01){
                //scene_.Update();

                // Animate the sphere
				std::string name = "TorusInstance";

				std::string curr_name;
				StaticEnemy *node;

				for (int i = 0; i < 4; i++)
				{
					curr_name = name + std::to_string(i + 1);
					node = (StaticEnemy*)scene_.GetNode(curr_name);
					std::cout << curr_name << "search!" << std::endl;
					if (node)
					{
						std::cout << curr_name << "FOUND!" << std::endl;
						node->StaticEnemy::Update(deltaTime);
					}
				}
                last_time = current_time;
            }
        }

        // Draw the scene
        scene_.Draw(&camera_);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    // Get user data with a pointer to the game class
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;

    // Quit game if 'q' is pressed
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    // Stop animation if space bar is pressed
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        game->animating_ = (game->animating_ == true) ? false : true;
    }

    // View control
    float rot_factor(glm::pi<float>() / 180);
    float trans_factor = 1.0;
    if (key == GLFW_KEY_UP){
        game->camera_.Pitch(rot_factor);
    }
    if (key == GLFW_KEY_DOWN){
        game->camera_.Pitch(-rot_factor);
    }
    if (key == GLFW_KEY_LEFT){
        game->camera_.Yaw(rot_factor);
    }
    if (key == GLFW_KEY_RIGHT){
        game->camera_.Yaw(-rot_factor);
    }
    if (key == GLFW_KEY_S){
        game->camera_.Roll(-rot_factor);
    }
    if (key == GLFW_KEY_X){
        game->camera_.Roll(rot_factor);
    }
    if (key == GLFW_KEY_A){
        game->camera_.Translate(game->camera_.GetForward()*trans_factor);
        game->skybox_->Translate(game->camera_.GetForward()*trans_factor);
    }
    if (key == GLFW_KEY_Z){
        game->camera_.Translate(-game->camera_.GetForward()*trans_factor);
        game->skybox_->Translate(-game->camera_.GetForward()*trans_factor);
    }
    if (key == GLFW_KEY_J){
        game->camera_.Translate(-game->camera_.GetSide()*trans_factor);
        game->skybox_->Translate(-game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_L){
        game->camera_.Translate(game->camera_.GetSide()*trans_factor);
        game->skybox_->Translate(game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_I){
        game->camera_.Translate(game->camera_.GetUp()*trans_factor);
        game->skybox_->Translate(game->camera_.GetUp()*trans_factor);
    }
    if (key == GLFW_KEY_K){
        game->camera_.Translate(-game->camera_.GetUp()*trans_factor);
        game->skybox_->Translate(-game->camera_.GetUp()*trans_factor);
    }
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height){

    // Set up viewport and camera projection based on new window size
    glViewport(0, 0, width, height);
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;
    game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


Game::~Game(){
    
    glfwTerminate();
}


Asteroid *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name){

    // Get resources
    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    // Create asteroid instance
    Asteroid *ast = new Asteroid(entity_name, geom, mat);
    scene_.AddNode(ast);
    return ast;
}


void Game::CreateAsteroidField(int num_asteroids){

    // Create a number of asteroid instances
    for (int i = 0; i < num_asteroids; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;

        // Create asteroid instance
        Asteroid *ast = CreateAsteroidInstance(name, "SimpleSphereMesh", "ObjectMaterial");

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum
        ast->SetPosition(glm::vec3(-300.0 + 600.0*((float) rand() / RAND_MAX), -300.0 + 600.0*((float) rand() / RAND_MAX), 600.0*((float) rand() / RAND_MAX)));
        ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
        ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
    }
}


SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, std::string envmap_name) {

	Resource *geom = resman_.GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	Resource *tex = NULL;
	if (texture_name != "") {
		tex = resman_.GetResource(texture_name);
		if (!tex) {
			throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
		}
	}

	Resource *envmap = NULL;
	if (envmap_name != "") {
		envmap = resman_.GetResource(envmap_name);
		if (!envmap) {
			throw(GameException(std::string("Could not find resource \"") + envmap_name + std::string("\"")));
		}
	}

	SceneNode *scn = scene_.CreateNode(entity_name, geom, mat, tex, envmap);
	return scn;
}

Enemy *Game::CreateEnemy(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, std::string envmap_name) {

	Resource *geom = resman_.GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	Resource *tex = NULL;
	if (texture_name != "") {
		tex = resman_.GetResource(texture_name);
		if (!tex) {
			throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
		}
	}

	Resource *envmap = NULL;
	if (envmap_name != "") {
		envmap = resman_.GetResource(envmap_name);
		if (!envmap) {
			throw(GameException(std::string("Could not find resource \"") + envmap_name + std::string("\"")));
		}
	}

	Enemy *scn = (Enemy*)scene_.CreateNode(entity_name, geom, mat, tex, envmap);
	return scn;
}

} // namespace game
