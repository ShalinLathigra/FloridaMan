#define GLM_FORCE_RADIANS

#include <iostream>
#include <sstream>
#include <time.h>

#include "bin/path_config.h"
#include "game.h"

namespace game
{
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
	glm::vec3 camera_position_g(0.0, 0.5, 0.0);
	glm::vec3 camera_look_at_g(0.0, 0.0, -10.0);
	glm::vec3 camera_up_g(0.0, 1.0, 0.0);

	// Materials
	const std::string material_directory_g = MATERIAL_DIRECTORY;

	Game::Game(void)
	{
		// Don't do work in the constructor, leave it for the Init() function
	}
	void Game::Init(void)
	{
		// Run all initialization steps
		InitWindow();
		InitView();
		InitEventHandlers();

		// Set variables
		animating_ = true;
		count_ = 0;
	}

	void Game::InitWindow(void)
	{
		// Initialize the window management library (GLFW)
		if (!glfwInit())
		{
			throw(GameException(std::string("Could not initialize the GLFW library")));
		}

		// Create a window and its OpenGL context
		if (window_full_screen_g)
		{
			window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		else
		{
			window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
		}
		if (!window_)
		{
			glfwTerminate();
			throw(GameException(std::string("Could not create window")));
		}

		// Make the window's context the current one
		glfwMakeContextCurrent(window_);

		// Initialize the GLEW library to access OpenGL extensions
		// Need to do it after initializing an OpenGL context
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			throw(GameException(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
		}
	}

	SceneGraph *Game::GetGraph()
	{
		return &scene_;
	}
	Player *Game::GetPlayer()
	{
		return player_;
	}
	void Game::InitView(void)
	{
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
		camera_.SetGame(this);
		// Set projection
		camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
	}

	void Game::InitEventHandlers(void)
	{
		// Set event callbacks
		glfwSetFramebufferSizeCallback(window_, ResizeCallback);
		glfwSetKeyCallback(window_, KeyCallback);

		// Set pointer to game object, so that callbacks can access it
		glfwSetWindowUserPointer(window_, (void *)this);
	}

	void Game::SetupResources(void)
	{
		// Can also check reflections on a cube
		//std::string cube_filename = std::string(MATERIAL_DIRECTORY) + std::string("/dense_cube.obj");
		//resman_.LoadResource(Mesh, "TorusMesh", cube_filename.c_str());

		// Create a torus
		resman_.CreateTorus("TorusMesh");

		resman_.CreateCylinder("CylinderMesh");
		// Use sphere to better analyze the environment map
		resman_.CreateSphere("SphereMesh");

		// Use sphere to better analyze the environment map
		resman_.CreateSphereParticles("SpherePartMesh", 500);

		// Use sphere to better analyze the environment map
		resman_.CreateSphereParticles("SimplePartMesh", 100);

		// Create a cube for the skybox
		resman_.CreateCube("CubeMesh");

		resman_.CreateWall("PlaneMesh");

		// Load material to be applied to torus
		std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/envmap");
		resman_.LoadResource(Material, "EnvMapMaterial", filename.c_str());

		filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
		resman_.LoadResource(Material, "MetalMaterial", filename.c_str());

		// Load material to be applied to torus
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/three-term_shiny_blue");
		resman_.LoadResource(Material, "ShinyMaterial", filename.c_str());
		// Load material to be applied to torus
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textured_material");
		resman_.LoadResource(Material, "TexturedMaterial", filename.c_str());
		// Load material to be applied to torus
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/toon_material");
		resman_.LoadResource(Material, "ToonMaterial", filename.c_str());

		filename = std::string(MATERIAL_DIRECTORY) + std::string("/force_material");
		resman_.LoadResource(Material, "ForceMaterial", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/bolt_material");
		resman_.LoadResource(Material, "BoltMaterial", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/particle_boom");
		resman_.LoadResource(Material, "ExplosionMaterial", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/particle_BIG_BOOM");
		resman_.LoadResource(Material, "BigExplosionMaterial", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/particle_spawn");
		resman_.LoadResource(Material, "SpawnMaterial", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/particle_alien");
		resman_.LoadResource(Material, "AlienMaterial", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/particle_trail");
		resman_.LoadResource(Material, "TrailMaterial", filename.c_str());

		// Load cube map to be applied to skybox
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/mp_sist/sist.tga");
		resman_.LoadResource(CubeMap, "LakeCubeMap", filename.c_str());

		filename = std::string(MATERIAL_DIRECTORY) + std::string("/ground.png");
		resman_.LoadResource(Texture, "GroundTexture", filename.c_str());

		filename = std::string(MATERIAL_DIRECTORY) + std::string("/alien.png");
		resman_.LoadResource(Texture, "AlienTexture", filename.c_str());

		// Load material to be applied to skybox
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/skybox");
		resman_.LoadResource(Material, "SkyboxMaterial", filename.c_str());
	}

	void Game::SetupScene(void)
	{
		// Set background color for the scene
		scene_.SetBackgroundColor(viewport_background_color_g);

		SceneNode *wall = CreateInstance(EntityType::Default, "PlaneInstance", "PlaneMesh", "TexturedMaterial", "GroundTexture");
		wall->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		wall->SetScale(glm::vec3(1000.0f));
		wall->SetOrientation(glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(1, 0, 0)));
		camera_.SetGround(wall);
		scene_.AddNode(wall);

		// Create skybox
		skybox_ = CreateInstance(EntityType::Default, "CubeInstance1", "CubeMesh", "SkyboxMaterial", "LakeCubeMap");
		skybox_->Scale(glm::vec3(1001.0f));
		skybox_->SetPosition(glm::vec3(0, 50, 0));
		skybox_->SetSkybox(true);
		scene_.AddNode(skybox_);
		camera_.SetSkyBox(skybox_);

		camera_.InitPlayer(&resman_);
		AddNode((SceneNode *)(camera_.GetPlayer()));
		player_ = camera_.GetPlayer();
		player_->Translate(glm::vec3(30, 6, -3));
		player_->SetGame(this);
		player_->SetCamera(&camera_);
		ParticleNode *part = (ParticleNode *)CreateInstance(EntityType::Particle, "boom_effect", "SpherePartMesh", "BigExplosionMaterial");
		part->SetBlending(true);
		player_->SetPart(part);

		SceneNode *hat = CreateInstance(EntityType::Default, "Hat", "SphereMesh", "ToonMaterial");
		hat->SetType(EntityType::Default);
		hat->SetScale(player_->GetScale() * 0.9f);
		hat->Translate(glm::vec3(0, 0.25, 0));
		player_->AddChild(hat);
		
		//ADD CAMO TEXTURE FOR FLORIDA MAN
		SceneNode *body = CreateInstance(EntityType::Default, "Body", "CubeMesh", "ToonMaterial");
		body->SetScale(glm::vec3(1.5f, 2.0f, 1.0));
		body->Translate(glm::vec3(0.0f, -1.5f, 0.0f));
		player_->AddChild(body);

		SceneNode *leftArm = CreateInstance(EntityType::Default, "LeftArm", "CubeMesh", "ToonMaterial");
		leftArm->SetScale(glm::vec3(1.5f, 0.75f, 1.0f));
		leftArm->Translate(glm::vec3(1.0f, 0.6f, 0.0));
		body->AddChild(leftArm);

		SceneNode *rightArm = CreateInstance(EntityType::Default, "RightArm", "CubeMesh", "ToonMaterial");
		rightArm->SetScale(glm::vec3(1.5f, 0.75f, 1.0f));
		rightArm->Translate(glm::vec3(-1.0f, 0.6f, 0.0));
		body->AddChild(rightArm);


		SceneNode *leftLeg = CreateInstance(EntityType::Default, "LeftLeg", "CubeMesh", "ToonMaterial");
		leftLeg->SetScale(glm::vec3(0.5f, 1.5f, 1.0f));
		leftLeg->Translate(glm::vec3(0.5f, -1.75f, 0.0));
		body->AddChild(leftLeg);

		SceneNode *rightLeg = CreateInstance(EntityType::Default, "RightLeg", "CubeMesh", "ToonMaterial");
		rightLeg->SetScale(glm::vec3(0.5f, 1.5f, 1.0f));
		rightLeg->Translate(glm::vec3(-0.5f, -1.75f, 0.0));
		body->AddChild(rightLeg);

		// TODO: CHAnge name of this pls;
		Asteroid *prop1 = (Asteroid*)CreateInstance(EntityType::AsteroidInst, "Prop1", "CubeMesh", "EnvMapMaterial", "", "LakeCubeMap");
		Asteroid *prop2 = (Asteroid*)CreateInstance(EntityType::AsteroidInst, "Prop2", "CubeMesh", "EnvMapMaterial", "", "LakeCubeMap");
		prop1->SetScale(glm::vec3(1.0, 0.1, 0.2));
		prop2->SetScale(glm::vec3(0.2, 0.1, 1.0));
		prop1->SetPosition(glm::vec3(0, hat->GetScale().y / 2.0f + 0.1, 0));
		prop2->SetPosition(glm::vec3(0, hat->GetScale().y / 2.0f + 0.1, 0));
		prop1->SetAngM(glm::angleAxis(glm::pi<float>() * 0.0125f, glm::vec3(0, 1, 0)));
		prop2->SetAngM(glm::angleAxis(glm::pi<float>() * 0.0125f, glm::vec3(0, 1, 0)));
		hat->AddChild(prop1);
		hat->AddChild(prop2);

		Asteroid *saucer = (Asteroid*)CreateInstance(EntityType::AsteroidInst, "FlyingTorus", "TorusMesh", "EnvMapMaterial", "", "LakeCubeMap");
		saucer->SetScale(glm::vec3(500));
		saucer->SetPosition(glm::vec3(0, 1000, 0)); 
		saucer->SetOrientation(glm::angleAxis(glm::pi<float>() / 3.0f, glm::vec3(1, 0, 0)));
		saucer->SetAngM(glm::angleAxis(glm::pi<float>() / 64.0f, glm::normalize(glm::vec3(0.2, 0.1, 1.0))));
		AddNode(saucer);

		CreateTowerField();
	}

	void Game::MainLoop(void)
	{

		// Loop while the user did not close the window
		while (!glfwWindowShouldClose(window_))
		{
			// Animate the scene
			if (animating_)
			{
				static double last_time = 0;
				double current_time = glfwGetTime();
				float deltaTime = current_time - last_time;
				if (deltaTime > 0.01)
				{
					GetKeyStates(window_);
					scene_.Update(deltaTime);
					last_time = current_time;

				}

				//Check for any nodes/entities to be removed
				scene_.RemoveNodes();

				// Draw the scene
				scene_.Draw(&camera_);

				// Push buffer drawn in the background onto the display
				glfwSwapBuffers(window_);

				// Update other events like input handling
				glfwPollEvents();
				if (alienCounter <= 0)
				{
					std::cout << "Florida Man freed all the aliens! You Win!!\n";
						break;
				}
				if (player_->GetHP() <= 0)
				{
					std::cout << "Florida Man was killed in the raid! You lost!\n";
					break;
				}
			}
		}
	}

	void Game::ResizeCallback(GLFWwindow *window, int width, int height)
	{
		// Set up viewport and camera projection based on new window size
		glViewport(0, 0, width, height);
		void *ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;
		game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
	}

	void Game::GetKeyStates(GLFWwindow *window)
	{
		// Get user data with a pointer to the game class
		void *ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;

		// Quit game if 'q' is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// View control
		float rot_factor(glm::pi<float>() / 360);
		float trans_factor = 1.0;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			game->camera_.Pitch(rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			game->camera_.Pitch(-rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			game->camera_.Yaw(rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			game->camera_.Yaw(-rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			game->camera_.Roll(-rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			game->camera_.Roll(rot_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			game->player_->Accelerate(-1.0);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			game->player_->Accelerate(1.0);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			game->camera_.Translate(-game->camera_.GetSide() * trans_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			game->camera_.Translate(game->camera_.GetSide() * trans_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			game->camera_.Translate(game->camera_.GetUp() * trans_factor);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			game->camera_.Translate(-game->camera_.GetUp() * trans_factor);
		}
	}

	void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

		// Get user data with a pointer to the game class
		void *ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;

		// Quit game if 'q' is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			game->player_->Fire(EntityType::BombProj);
		}

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			game->player_->Fire(EntityType::ShurikenProj);
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			game->player_->Fire(EntityType::MineProj);
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			game->camera_.TogglePOV();
		}
	}

	Game::~Game()
	{
		glfwTerminate();
	}

	SceneNode *Game::CreateInstance(int type, std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, std::string envmap_name)
	{
		Resource *geom = resman_.GetResource(object_name);
		if (!geom)
		{
			throw(GameException(std::string("Could not find Geometry \"") + object_name + std::string("\"")));
		}

		Resource *mat = resman_.GetResource(material_name);
		if (!mat)
		{
			throw(GameException(std::string("Could not find Material \"") + material_name + std::string("\"")));
		}

		Resource *tex = NULL;
		if (texture_name != "")
		{
			tex = resman_.GetResource(texture_name);
			if (!tex)
			{
				throw(GameException(std::string("Could not find Texture \"") + material_name + std::string("\"")));
			}
		}

		Resource *envmap = NULL;
		if (envmap_name != "")
		{
			envmap = resman_.GetResource(envmap_name);
			if (!envmap)
			{
				throw(GameException(std::string("Could not find Envmap \"") + envmap_name + std::string("\"")));
			}
		}
		SceneNode *scn = scene_.CreateNode(type, entity_name, geom, mat, tex, envmap);
		return scn;
	}

	SceneNode *Game::CreateEntity(int type, glm::vec3 pos, glm::vec3 scale)
	{
		std::string entity_name, object_name, material_name, texture_name, envmap_name;
		bool isEnemy = true;
		bool isProj = false;
		bool child = false;
		switch (type)
		{
		case (Turret):
			entity_name = std::string("TurretNode") + std::to_string(count_++);
			object_name = std::string("CubeMesh");
			material_name = std::string("ToonMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			child = true;
			break;
		case (Ground):
			entity_name = std::string("GroundEntity") + std::to_string(count_++);
			object_name = std::string("SphereMesh");
			material_name = std::string("ToonMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			break;
		case (Air):
			entity_name = std::string("AirEntity") + std::to_string(count_++);
			object_name = std::string("SphereMesh");
			material_name = std::string("ToonMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			break;
		case (MineProj):
			entity_name = std::string("Mine") + std::to_string(count_++);
			object_name = std::string("SphereMesh");
			material_name = std::string("ToonMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			isEnemy = false;
			break;
		case (BombProj):
			entity_name = std::string("Bomb") + std::to_string(count_++);
			object_name = std::string("CylinderMesh");
			material_name = std::string("ToonMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			isEnemy = false;
			break;
		case (ShurikenProj):
			entity_name = std::string("Shuriken") + std::to_string(count_++);
			object_name = std::string("SphereMesh");
			material_name = std::string("ToonMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			isEnemy = false;
			isProj = true;
			break;
		case (MissileProj):
			entity_name = std::string("Shuriken") + std::to_string(count_++);
			object_name = std::string("TorusMesh");
			material_name = std::string("BoltMaterial");
			texture_name = std::string("");
			envmap_name = std::string("");
			isEnemy = false;
			isProj = true;
			break;
		default:
			entity_name = std::string("SceneNode") + std::to_string(count_++);
			object_name = std::string("CubeMesh");
			material_name = std::string("EnvMapMaterial");
			texture_name = std::string("");
			envmap_name = std::string("LakeCubeMap");
			break;
		}

		SceneNode *scn = (SceneNode *)CreateInstance(type, entity_name, object_name, material_name, texture_name, envmap_name);

		if (child)
		{
			SceneNode *p = CreateInstance(EntityType::Default, std::string("Stand") + std::to_string(count_), std::string("CubeMesh"), std::string("ShinyMaterial"), "", "");
			p->AddChild(scn);
			p->SetPosition(glm::vec3(pos.x, p->GetScale().y / 2.0f, pos.z));
			scn->SetPosition(glm::vec3(0, (p->GetScale().y) / 2.0f + scale.y, 0));
		}
		else
		{
			scn->SetPosition(pos);
		}
		scn->SetScale(scale);
		scn->SetGame(this);

		if (isEnemy)
		{
			((Entity *)scn)->SetTarget(camera_.GetPlayer());
		}
		if (isProj)
		{
			ParticleNode *p = (ParticleNode *)CreateInstance(EntityType::Particle, std::string("_trail_)") + std::to_string(count_), std::string("SimplePartMesh"), std::string("TrailMaterial"));
			p->Start();
			p->SetBlending(true);
			scn->AddChild(p);
		}
		return scn;
	}

	void Game::AddNode(SceneNode *scn)
	{
		scene_.AddNode(scn);
		if (scn->GetName().find("Entity") != std::string::npos)
		{
			ParticleNode *part = (ParticleNode *)CreateInstance(EntityType::Particle, "PartInstance1", "SpherePartMesh", "SpawnMaterial");
			part->SetPosition(scn->GetPosition());
			scene_.AddNode(part);
		}
	}

	Camera *Game::GetCamera(void)
	{
		return &camera_;
	}

	void Game::AlienFreed()
	{
		alienCounter -= 1;
	}
	void Game::CreateTowerField(void)
	{
		float range = 750;
		float dim = 150;
		SceneNode *scn;
		glm::vec3 scale;
		std::string object_name, material_name, texture_name, envmap_name;

		for (float x = -range / dim; x < range / dim; x += 1.0f)
		{
			for (float z = -range / dim; z < range / dim; z += 1.0f)
			{
				int type = utilities::RandPercent() * 3.9f;

				std::string suffix = std::to_string((int)x) + std::to_string((int)z);
				scale = glm::vec3(14, 20, 14) + glm::vec3(14, 20, 14) * utilities::RandPercent();

				std::string entity_name = "Tower: " + suffix;
				if (type >= 3)
				{
					entity_name = "AlienEntity_" + suffix;
					alienCounter += 1;
				}

				scn = CreateInstance(TurretSpawn + type, entity_name, "CylinderMesh", "ToonMaterial");
				scn->SetOrientation(utilities::RandQuat(glm::vec3(0, 1, 0)));
				scn->SetScale(scale);
				scn->SetPosition(glm::vec3(x * dim, scale.y / 2.0f, z * dim));
				scn->SetType(type);
				scn->SetGame(this);

				if (type < 3)
				{
					switch (type)
					{
					case (Turret):
						object_name = std::string("TorusMesh");
						break;
					case (Ground):
						object_name = std::string("TorusMesh");
						break;
					case (Air):
						object_name = std::string("SphereMesh");
					}

					material_name = std::string("ToonMaterial");
					texture_name = std::string("");
					envmap_name = std::string("");

					Resource *geom = resman_.GetResource(object_name);
					if (!geom)
					{
						throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
					}
					Resource *mat = resman_.GetResource(material_name);
					if (!mat)
					{
						throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
					}
					Resource *tex = NULL;
					if (texture_name != "")
					{
						tex = resman_.GetResource(texture_name);
						if (!tex)
						{
							throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
						}
					}
					Resource *envmap = NULL;
					if (envmap_name != "")
					{
						envmap = resman_.GetResource(envmap_name);
						if (!envmap)
						{
							throw(GameException(std::string("Could not find resource \"") + envmap_name + std::string("\"")));
						}
					}
					ParticleNode *part = (ParticleNode *)CreateInstance(EntityType::Particle, "boom_effect", "SpherePartMesh", "ExplosionMaterial");
					part->SetBlending(true);
					((EntityStructure *)scn)->InitResources(type, geom, mat, tex, envmap, *part);
				}
				else
				{

					ParticleNode *part = (ParticleNode *)CreateInstance(EntityType::Particle, "PartInstance1", "SpherePartMesh", "AlienMaterial", "AlienTexture");
					part->SetBlending(true);
					part->SetDuration(12.0f);
					((Entity*)scn)->SetDeathEffect(*part);
				}

				AddNode(scn);
			}
		}
	}
} // namespace game