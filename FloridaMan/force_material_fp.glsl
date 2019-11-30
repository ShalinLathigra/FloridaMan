// Material with no illumination simulation

#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;

// Material attributes (constants)
vec4 ambient_albedo = vec4(0.0, 0.0, 1.0, 1.0);
vec4 diffuse_albedo = vec4(0.0, 0.0, 0.7, 1.0);
vec4 specular_albedo = vec4(0.8, 0.5, 0.9, 1.0);
float phong_exponent = 64.0;
float Ia = 0.2; // Ambient light amount

uniform int type;
uniform float timer;
float fresnel_exponent = 1;
float amb = 0.05;


void main() 
{
    vec3 N, // Interpolated normal for fragment
         V; // View direction

    N = normalize(normal_interp);
	
    V = - position_interp; // Eye position is (0, 0, 0) in view coordinates
    V = normalize(V);

	float fresnel = 1-pow(max(dot(N, V), -dot(N, V)), fresnel_exponent);
	float x = clamp((timer) * tan(position_interp.y * position_interp.z), 0, 1);

	gl_FragColor = x * (fresnel+amb) * vec4(1, 0.05, 0.05, 1);
}
