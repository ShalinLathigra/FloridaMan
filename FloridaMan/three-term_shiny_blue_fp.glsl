// Illumination based on the traditional three-term model

#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec3 light_pos;

// Material attributes (constants)
vec4 ambient_albedo = vec4(0.0, 0.0, 1.0, 1.0);
vec4 diffuse_albedo = vec4(0.0, 0.0, 0.7, 1.0);
vec4 specular_albedo = vec4(0.8, 0.5, 0.9, 1.0);
float phong_exponent = 64.0;
float Ia = 0.2; // Ambient light amount

uniform int type;
void main() 
{
    vec3 N, // Interpolated normal for fragment
         L, // Light-source direction
         V, // View direction
         H; // Half-way vector

    // Compute Lambertian term Id
    N = normalize(normal_interp);

    L = (light_pos - position_interp);
    L = normalize(L);

    float Id = max(dot(N, L), 0.0);
    
    // Compute specular term Is for Blinn-Phong shading
    // V = (eye_position - position_interp);
    V = - position_interp; // Eye position is (0, 0, 0) in view coordinates
    V = normalize(V);

    //H = 0.5*(V + L); // Halfway vector
    H = (V + L); // Halfway vector (will be normalized anyway)
    H = normalize(H);

    float spec_angle_cos = max(dot(N, H), 0.0);
    float Is = pow(spec_angle_cos, phong_exponent);
        
    // Assign illumination to the fragment
    gl_FragColor = Ia*ambient_albedo + Id*diffuse_albedo + Is*specular_albedo;
    if (type == 0)
	{
	gl_FragColor = gl_FragColor.bbga;
	}
	if (type == 1)
	{
	gl_FragColor = gl_FragColor.brga;
	}
	if (type == 2)
	{
	gl_FragColor = gl_FragColor.rbga;
	}
    // For debug, we can display the different values
    //gl_FragColor = ambient_albedo;
    //gl_FragColor = diffuse_albedo;
    //gl_FragColor = specular_albedo;
    //gl_FragColor = albedo_interp;
    //gl_FragColor = vec4(N.xyz, 1.0);
    //gl_FragColor = vec4(L.xyz, 1.0);
    //gl_FragColor = vec4(V.xyz, 1.0);
}
