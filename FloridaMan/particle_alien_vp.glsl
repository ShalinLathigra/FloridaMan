#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;
uniform float start;
uniform float duration;

// Attributes forwarded to the geometry shader
out vec4 particle_color;
out float particle_id;
out float particle_step;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0); // Up direction
uniform vec3 back_vec = vec3(0.0, 0.0, -1.0); // Up direction
float up_acc = 0.01; // An acceleration applied to the particles coming from some attraction force
float back_acc = 0.05; // An acceleration applied to the particles coming from some attraction force

float speed = 25.0; // Control the speed of the motion

const vec4 mothership = vec4(0, 10, 0, 1);

// Define some useful constants
const float pi = 3.1415926536;
const float pi_over_two = 1.5707963268;
const float two_pi = 2.0*pi;


void main()
{
    // Define particle id
    particle_id = color.r; // Derived from the particle color. We use the id to keep track of particles

	// Let time cycle every four seconds
    float t = timer - start; // Our time parameter
    
    // Forward time step to geometry shader
    particle_step = t / duration;

    // Let's first work in model space (apply only world matrix)
    vec4 position = world_mat * vec4(vertex, 1.0);
    vec4 norm = normal_mat * vec4(normal, 1.0);

    // Move point along normal and down with t*t (acceleration under gravity)
	vec3 temp = vec3(position.x, 0.5, position.z);
    position.y = 0.5;
	
	//Want X to increase, then decrease based on time
	//Want Z to increase, then decrease based on time
	//Want Y to sloooowly increase, then shoot up.
	
	position.x = position.x + speed * t * norm.x * sin(particle_step * two_pi / 2);
	position.z = position.z + speed * t * norm.z * sin(particle_step * two_pi / 2);
	float y = position.y + pow((max(t - duration/2, 0)), 2) * speed;
	
	temp -= position.xyz;
	float val = clamp(0.005 * (pow(1-temp.x, 2) + pow(1-temp.z, 2)), 0, 1);

	position.y = (val)*position.y + (1-val)*y;





	
    // Now apply view transformation
    gl_Position = view_mat * position;
    // Define amount of blending depending on the cyclic time
    float alpha = 0.5 * particle_step;
	particle_color = vec4(val, val, val,1);
}
