#version 400

in vec3 FragPosition;

out vec4 FragColor;

uniform samplerCube Cubemap;

void main()
{
	vec3 EnvColor = texture(Cubemap, FragPosition).rgb;
	//vec3 EnvColor = textureLod(Cubemap, FragPosition, 1).rgb;
    EnvColor = EnvColor / (EnvColor + vec3(1.0));
    EnvColor = pow(EnvColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(EnvColor, 1.0);
}
