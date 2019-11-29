// Illumination based on the traditional three-term model

#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec3 light_pos;

// Material attributes (constants)
vec4 amb = vec4(1.0, 0.1, 0.2, 1.0);
vec4 diff = vec4(0.7, 0.3, 0.1, 1.0);
vec4 spec = vec4(1.0, 0.7, 0.5, 1.0);

float phong_exponent = 64.0;
float Ia = 0.4; // Ambient light amount

//	Define Different Colours, toggle between them.

float Ds = 2.0;
float Ss = 2.0;

vec4 outline_color = vec4(0.0, 0.0, 0.0, 1.0);
float outline_strength = 0.2;

uniform int type;

void main() 
{
    vec3 N, // Interpolated normal for fragment
         L, // Light-source direction
         V, // View direction
         R; // Reflected Light vector

    // Compute Lambertian term Id
    N = normalize(normal_interp);

    L = (light_pos - position_interp);
    L = normalize(L);

    float Id = max(dot(N, L), 0.0);

    // Compute specular term Is for Blinn-Phong shading
    // V = (eye_position - position_interp);
    V = - position_interp; // Eye position is (0, 0, 0) in view coordinates
    V = normalize(V);

    //R = -L + 2*(dot(N, L)*N); // Reflected Light Vector
    R = -L + 2*(dot(N, L)*N); // Reflected Light Vector (will be normalized anyway)
    R = normalize(R);

    float spec_angle_cos = max(dot(N, R), 0.0);
    float Is = pow(spec_angle_cos, phong_exponent);

	//Apply Toon Shading Effects to Diffuse and Specular Light
	Id = int(Ds*Id + 0.5) / Ds;
	Is = int(Ss*Is + 0.5) / Ss;

	//Apply Outline
	//Need to pass in the actual direction of the eye.
    float O = int(1-max(dot(V, N), 0.0)+outline_strength);
	gl_FragColor = (O*outline_color) + (1.0 - O) * (Ia*amb + Id*diff + Is*spec);

	if (type == 0)
	{
		gl_FragColor = gl_FragColor.rgba;
	}
	else if (type == 1)
	{
		gl_FragColor = gl_FragColor.rrga;
	}
	else if (type == 2)
	{
		gl_FragColor = gl_FragColor.brga;
	}
	else if (type == 3)
	{
		gl_FragColor = gl_FragColor.bgra;
	}
	else if (type == 13)
	{
		gl_FragColor = gl_FragColor.brra;
	}
	else 
	{
		gl_FragColor = gl_FragColor.ggga;
	}

    // For debug, we can display the different values
    //gl_FragColor = ambient_color;
    //gl_FragColor = diffuse_color;
    //gl_FragColor = specular_color;
    //gl_FragColor = color_interp;
    //gl_FragColor = vec4(N.xyz, 1.0);
    //gl_FragColor = vec4(L.xyz, 1.0);
    //gl_FragColor = vec4(V.xyz, 1.0);
}
