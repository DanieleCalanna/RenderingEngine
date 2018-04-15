#version 400 core

out vec4 FragColor;
in vec3 Position_WS;

uniform sampler2D EquirectangularMap;

const vec2 InvAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
	vec2 UV = vec2(atan(v.z, v.x), asin(v.y));
	UV *= InvAtan;
	UV += 0.5;
	return UV;
}

void main()
{
	vec2 UV = SampleSphericalMap(normalize(Position_WS));
	FragColor = vec4(texture(EquirectangularMap, UV).rgb, 1.0);
}