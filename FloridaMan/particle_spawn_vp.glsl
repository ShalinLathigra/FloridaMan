#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float start;
uniform float timer;
uniform float duration;

// Attributes forwarded to the geometry shader
out vec3 vertex_color;
out float timestep;


// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.8, 0.8, 0.8);
float speed = 45.0; // Allows to control the speed of the explosion

void main()
{
	float lifetime = duration;

    // Let time cycle every four seconds
	float t = max(duration - (timer - start), 0); // Our time parameter
    
    // Let's first work in model space (apply only world matrix)
    vec4 position = world_mat * vec4(vertex, 1.0);
    vec4 norm = normal_mat * vec4(normal, 1.0);

    // Move point along normal and down with t*t (acceleration under gravity)
    position.x += norm.x*color.g*t*speed;
    position.y += norm.y*color.g*t*speed;
    position.z += norm.z*color.g*t*speed;
    
    // Now apply view transformation
    gl_Position = view_mat * position;
        
    // Define outputs
    // Define color of vertex

	float net = clamp(pow(norm.x*color.g, 2)*t*speed + pow(norm.y*color.g, 2)*t*speed + pow(norm.z*color.g, 2)*t*speed, 0, 1);

	vec3 color_ = vec3(color.r, color.g*10, color.b);
	vertex_color = (net) * color_.rgb + (1-net)*color_.grr;

    //vertex_color = color.rgb; // Color defined during the construction of the particles
    //vertex_color = object_color * (1-pow(circtime / lifetime, 2)); // Uniform color 
    //vertex_color = vec3(t, 0.0, 1-t);
    //vertex_color = vec3(1.0, 1-t, 0.0);

    // Forward time step to geometry shader
    timestep = clamp(timer - start, 0, 1);
}
