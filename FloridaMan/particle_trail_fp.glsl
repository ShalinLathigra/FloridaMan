#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 tex_coord;

void main (void)
{
    // Very simple fragment shader, but we can do anything we want here
    // We could apply a texture to the particle, illumination, etc.
	
	gl_FragColor = (1 - vec4(pow(2 * (tex_coord.x-.5), 2) + pow(2 * (tex_coord.y-.5), 2))) * frag_color;	
    //gl_FragColor = 16 * (pow(0.25, 2) - pow(tex_coord.x-.5, 2)*pow(tex_coord.y-.5, 2))*frag_color;
}
