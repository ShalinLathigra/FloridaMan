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

	Game::Game(void) {

		// Don't do work in the constructor, leave it for the Init() function
	}
	void Game::Init(void) {

		// Run all initialization steps
		InitWindow();
		InitView();
		InitEventHandlers();

		// Set variables
		animating_ = true;
		count_ = 0;
	}


	void Game::InitWindow(void) {

		// Initialize the window management library (GLFW)
		if (!glfwInit()) {
			throw(GameException(std::string("Could not initialize the GLFW library")));
		}

		// Create a window and its OpenGL context
		if (window_full_screen_g) {
			window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		else {
			window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
		}
		if (!window_) {
			glfwTerminate();
			throw(GameException(std::string("Could not create window")));
		}

		// Make the window's context the current one
		glfwMakeContextCurrent(window_);

		// Initialize the GLEW library to access OpenGL extensions
		// Need to do it after initializing an OpenGL context
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			throw(GameException(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
		}
	}

	void Game::InitView(void) {

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

	void Game::InitEventHandlers(void) {

		// Set event callbacks
		glfwSetFramebufferSizeCallback(window_, ResizeCallback);

		// Set pointer to game object, so that callbacks can access it
		glfwSetWindowUserPointer(window_, (void *)this);
	}

	void Game::SetupResources(void) {

		// Can also check reflections on a cube
		//std::string cube_filename = std::string(MATERIAL_DIRECTORY) + std::string("/dense_cube.obj");
		//resman_.LoadResource(Mesh, "TorusMesh", cube_filename.c_str());

		// Create a torus
		resman_.CreateTorus("TorusMesh");

		// Use sphere to better analyze the environment map
		resman_.CreateSphere("SphereMesh");

		// Create a cube for the skybox
		resman_.CreateCube("CubeMesh");

		resman_.CreateWall("PlaneMesh");

		// Load material to be applied to torus
		std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/envmap");
		resman_.LoadResource(Material, "EnvMapMaterial", filename.c_str());

		// Load material to be applied to torus
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/three-term_shiny_blue");
		resman_.LoadResource(Material, "ShinyMaterial", filename.c_str());
		// Load material to be applied to torus
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textured_material");
		resman_.LoadResource(Material, "TexturedMaterial", filename.c_str());

		// Load cube map to be applied to skybox
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/mp_sist/sist.tga");
		resman_.LoadResource(CubeMap, "LakeCubeMap", filename.c_str());

		// Load cube map to be applied to skybox
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/ground.png");
		resman_.LoadResource(Texture, "GroundTexture", filename.c_str());

		// Load material to be applied to skybox
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/skybox");
		resman_.LoadResource(Material, "SkyboxMaterial", filename.c_str());
	}


	void Game::SetupScene(void) {

		//size = 1000x1000
		// divide into 10x10 regions
		// size = 100x100
		float dim = 1000.0f;
		float scale = 75.0f;
		for (float x = -dim/scale; x < dim / scale; x++)
		{
			for (float z = -dim / scale; z < dim / scale; z++)
			{
				//random number 1 to 3.
				;
				int current = ((int)x * (int)z) % 4;
				switch (current)
				{
				case(0):
					CreateEntity(EntityType::Destructible, glm::vec3(x * scale, 10, z * scale), glm::vec3(10, 20, 10));
					break;
				case(1):
					CreateEntity(EntityType::Static, glm::vec3(x * scale, 0.0f, z * scale), glm::vec3(6));
					break;
				case(2):
					CreateEntity(EntityType::Ground, glm::vec3(x * scale, 0.0f, z * scale), glm::vec3(7, 5, 30));
					break;
				case(3):
					CreateEntity(EntityType::Air, glm::vec3(x * scale, scale, z * scale), glm::vec3(10.0, 1.0, 15.0));
					break;
				}
			}
		}

		// Set background color for the scene
		scene_.SetBackgroundColor(viewport_background_color_g);

		//CreateEntity(EntityType::Static, glm::vec3(1.0, 1.0, -10.0), glm::vec3(6, 6, 6));
		//
		//CreateEntity(EntityType::Ground, glm::vec3(1.0, 0.725, -10.0), glm::vec3(1.0, 1.5, 1.5));
		//CreateEntity(EntityType::Air, glm::vec3(1.0, 5.0, -10.0), glm::vec3(1.5, 0.5, 1.5));
		//
		//CreateEntity(EntityType::Destructible, glm::vec3(-13, 10.0, -10), glm::vec3(10.0f, 20.0f, 10.0f));


		SceneNode *wall = CreateInstance(EntityType::Default, "PlaneInstance", "PlaneMesh", "TexturedMaterial", "LakeCubeMap");
		wall->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		wall->SetScale(glm::vec3(1000.0f));
		wall->SetOrientation(glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(1, 0, 0)));

		// Create skybox
		skybox_ = CreateInstance(EntityType::Default, "CubeInstance1", "CubeMesh", "SkyboxMaterial", "LakeCubeMap");
		skybox_->Scale(glm::vec3(1001.0f));
		skybox_->SetPosition(glm::vec3(0, 50, 0));
		skybox_->SetSkybox(true);
	}

	/*
	Destructible Buildings
		Alien particle effects
			Animated to run!
			disappear in a ring -explosion dealio
	*/

	void Game::MainLoop(void) {

		// Loop while the user did not close the window
		while (!glfwWindowShouldClose(window_)) {
			// Animate the scene
			if (animating_) {
				static double last_time = 0;
				double current_time = glfwGetTime();
				float deltaTime = current_time - last_time;
				if (deltaTime > 0.01) {
					GetKeyStates(window_);
					scene_.Update(deltaTime);
					last_time = current_time;
				}

				// Draw the scene
				scene_.Draw(&camera_);

				// Push buffer drawn in the background onto the display
				glfwSwapBuffers(window_);

				// Update other events like input handling
				glfwPollEvents();
			}
		}
	}

	void Game::ResizeCallback(GLFWwindow* window, int width, int height) {

		// Set up viewport and camera projection based on new window size
		glViewport(0, 0, width, height);
		void* ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;
		game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
	}

	void Game::GetKeyStates(GLFWwindow* window) {

		// Get user data with a pointer to the game class
		void* ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;

		// Quit game if 'q' is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		// Stop animation if space bar is pressed
		//if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		//	 game->animating_ = (game->animating_ == true) ? false : true;
		//}

		// View control
		float rot_factor(glm::pi<float>() / 360);
		float trans_factor = 0.5;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			game->camera_.Pitch(rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			game->camera_.Pitch(-rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			game->camera_.Yaw(rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			game->camera_.Yaw(-rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			game->camera_.Roll(-rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			game->camera_.Roll(rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			game->camera_.Translate(game->camera_.GetForward()*trans_factor);
			game->skybox_->Translate(game->camera_.GetForward()*trans_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			game->camera_.Translate(-game->camera_.GetForward()*trans_factor);
			game->skybox_->Translate(-game->camera_.GetForward()*trans_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			game->camera_.Translate(-game->camera_.GetSide()*trans_factor);
			game->skybox_->Translate(-game->camera_.GetSide()*trans_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			game->camera_.Translate(game->camera_.GetSide()*trans_factor);
			game->skybox_->Translate(game->camera_.GetSide()*trans_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			game->camera_.Translate(game->camera_.GetUp()*trans_factor);
			game->skybox_->Translate(game->camera_.GetUp()*trans_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			game->camera_.Translate(-game->camera_.GetUp()*trans_factor);
			game->skybox_->Translate(-game->camera_.GetUp()*trans_factor);
		}

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			//game->CreateEntity(EntityType::MineInstance, game->camera_.GetPosition(), glm::vec3(1.5));
		}
	}

	Game::~Game() {

		glfwTerminate();
	}


	Asteroid *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name) {

		// Get resources
		Resource *geom = resman_.GetResource(object_name);
		if (!geom) {
			throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
		}

		Resource *mat = resman_.GetResource(material_name);
		if (!mat) {
			throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
		}

		// Create asteroid instance
		Asteroid *ast = new Asteroid(entity_name, geom, mat);
		scene_.AddNode(ast);
		return ast;
	}


	void Game::CreateAsteroidField(int num_asteroids) {

		// Create a number of asteroid instances
		for (int i = 0; i < num_asteroids; i++) {
			// Create instance name
			std::stringstream ss;
			ss << i;
			std::string index = ss.str();
			std::string name = "AsteroidInstance" + index;

			// Create asteroid instance
			Asteroid *ast = CreateAsteroidInstance(name, "SimpleSphereMesh", "ObjectMaterial");

			// Set attributes of asteroid: random position, orientation, and
			// angular momentum
			ast->SetPosition(glm::vec3(-300.0 + 600.0*((float)rand() / RAND_MAX), -300.0 + 600.0*((float)rand() / RAND_MAX), 600.0*((float)rand() / RAND_MAX)));
			ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
			ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		}
	}


	SceneNode *Game::CreateInstance(int type, std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, std::string envmap_name, bool add) {

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
		SceneNode *scn = scene_.CreateNode(type, entity_name, geom, mat, tex, envmap, add);
		return scn;
	}


	void Game::CreateEntity(int type, glm::vec3 pos, glm::vec3 scale)
	{

		std::string entity_name, object_name, material_name, texture_name, envmap_name;
		bool isEnemy = true;
		bool child = false;
		switch (type)
		{
		case(Static):
			entity_name = std::string("StaticEntity") + std::to_string(count_++);
			object_name = std::string("CubeMesh");
			material_name = std::string("EnvMapMaterial");
			texture_name = std::string("");
			envmap_name = std::string("LakeCubeMap");
			child = true;
			break;
		case(Ground):
			entity_name = std::string("GroundEntity") + std::to_string(count_++);
			object_name = std::string("TorusMesh");
			material_name = std::string("EnvMapMaterial");
			texture_name = std::string("");
			envmap_name = std::string("LakeCubeMap");
			break;
		case(Air):
			entity_name = std::string("AirEntity") + std::to_string(count_++);
			object_name = std::string("SphereMesh");
			material_name = std::string("ShinyMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			break;
		case(MineInstance):
			entity_name = std::string("MineInstance") + std::to_string(count_++);
			object_name = std::string("SphereMesh");
			material_name = std::string("ShinyMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			isEnemy = false;
			break;
		case(Destructible):
			entity_name = std::string("DestructibleInstance") + std::to_string(count_++);
			object_name = std::string("CubeMesh");
			material_name = std::string("ShinyMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			isEnemy = false;
			break;
		}

		SceneNode *scn = (SceneNode*)CreateInstance(type, entity_name, object_name, material_name, texture_name, envmap_name, !child);

		if (child)
		{
			SceneNode *p = CreateDestructibleDefault();
			p->AddChild(scn);
			p->SetPosition(glm::vec3(pos.x, p->GetScale().y/2.0f, pos.z));
			scn->SetPosition(glm::vec3(0, (p->GetScale().y) / 2.0f + scale.y, 0));
		}
		else
		{
			scn->SetPosition(pos);
		}

		scn->SetScale(scale);

		if (isEnemy) {
			((Entity*)scn)->SetTarget(&camera_);
			((Entity*)scn)->SetGame(this);
		}
	}

	SceneNode *Game::CreateDestructibleDefault(void) {

		std::string entity_name = std::string("DestructibleInstance") + std::to_string(count_);
		std::string object_name = std::string("CubeMesh");
		std::string material_name = std::string("ShinyMaterial");
		std::string texture_name = std::string("");
		std::string envmap_name = std::string("");

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
		SceneNode *scn = scene_.CreateNode(Destructible, entity_name, geom, mat, tex, envmap);
		scn->SetPosition(glm::vec3(-13, 10.0, -10));
		scn->SetScale(glm::vec3(10.0f, 20.0f, 10.0f));
		count_++;
		return scn;
	}
} // namespace game
