#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 tex_coord;
in float time_step;

void main (void)
{
    // Very simple fragment shader, but we can do anything we want here
    // We could apply a texture to the particle, illumination, etc.

	vec4 shape = (1 - vec4(pow(2 * (tex_coord.x-.5), 2) + pow(2 * (tex_coord.y-.5), 2)));
	gl_FragColor = shape * (0.1 * vec4(1, 0.75, 0.01, 1) + (1-time_step) * vec4(1, 0.01, 0.05, 1) + time_step * frag_color) * time_step;	
}
