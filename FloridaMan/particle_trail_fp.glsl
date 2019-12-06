#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 tex_coord;

void main (void)
{
	gl_FragColor = (1 - vec4(pow(2 * (tex_coord.x-.5), 2) + pow(2 * (tex_coord.y-.5), 2))) * frag_color;	
}
