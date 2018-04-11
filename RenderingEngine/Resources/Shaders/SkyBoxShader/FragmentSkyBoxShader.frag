#version 400

out vec4 FragColor;

in vec3 texcoords;

uniform samplerCube CubeMap;

void main()
{
	vec3 EnvColor = texture(CubeMap, texcoords).rgb;
    EnvColor = EnvColor / (EnvColor + vec3(1.0));
    EnvColor = pow(EnvColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(EnvColor, 1.0);
}
