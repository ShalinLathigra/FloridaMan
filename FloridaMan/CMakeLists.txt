cmake_minimum_required(VERSION 2.6)

# Name of project
set(PROJ_NAME FloridaMan)
project(${PROJ_NAME})

# Specify project files: header files and source files
set(HDRS
	air_entity.h
	asteroid.h 
	bomb.h
	entity.h 
	entity_structure.h 
	camera.h 
	game.h 
	ground_entity.h 
	mine.h
	model_loader.h 
	particle_node.h  
	player.h
	resource.h 
	resource_manager.h 
	scene_graph.h 
	scene_node.h
	shuriken.h
	turret_node.h
	utilities.h
)
 
set(SRCS
	air_entity.cpp 
	asteroid.cpp 
	bomb.cpp
	camera.cpp 
	entity.cpp 
	entity_structure.cpp 
	game.cpp 
	ground_entity.cpp 
	main.cpp
	player.cpp 
	mine.cpp
	particle_node.cpp
	resource.cpp 
	resource_manager.cpp 
	scene_graph.cpp 
	scene_node.cpp 
	shuriken.cpp
	turret_node.cpp
	utilties.cpp

	CMakeLists.txt
)
set(SRC_SHADERS 
	envmap_vp.glsl 
	envmap_fp.glsl 
	force_material_vp.glsl 
	force_material_fp.glsl
	material_fp.glsl 
	material_vp.glsl 
	metal_fp.glsl 
	metal_vp.glsl 
	normal_map_vp.glsl 
	normal_map_fp.glsl 
	plastic_fp.glsl 
	plastic_vp.glsl 
	screen_space_vp.glsl 
	screen_space_fp.glsl 
	skybox_vp.glsl 
	skybox_fp.glsl 
	textured_material_fp.glsl 
	textured_material_vp.glsl 
	three-term_shiny_blue_fp.glsl 
	three-term_shiny_blue_vp.glsl 
	toon_material_fp.glsl 
	toon_material_vp.glsl 
	
	fire_vp.glsl 
	fire_gp.glsl 
	fire_fp.glsl
	particle_alien_vp.glsl 
	particle_alien_fp.glsl 
	particle_alien_gp.glsl 
	particle_boom_vp.glsl 
	particle_boom_fp.glsl 
	particle_boom_gp.glsl 
	particle_spawn_vp.glsl 
	particle_spawn_fp.glsl 
	particle_spawn_gp.glsl 
	particle_trail_vp.glsl 
	particle_trail_fp.glsl 
	particle_trail_gp.glsl 
)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS} ${SRC_SHADERS})
source_group("Shaders" FILES ${SRC_SHADERS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH "" CACHE PATH "Folder with GLEW, GLFW, GLM, and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
