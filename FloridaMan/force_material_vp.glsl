// Illumination based on the traditional three-term model

#version 130

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
uniform mat4 normal_view_mat;

// Attributes forwarded to the fragment shader
out vec3 position_interp;
out vec3 normal_interp;

// Material attributes (constants)
//
// Could be loaded from a configuration file and also passed with the
// uniform buffer


void main()
{
    // Transform vertex position
    gl_Position = projection_mat * view_mat * world_mat * vec4(vertex, 1.0);

    // Transform vertex position without including projection
    position_interp = vec3(view_mat * world_mat * vec4(vertex, 1.0));

    // Transform normal
    normal_interp = vec3(normal_view_mat * vec4(normal, 0.0));

}
