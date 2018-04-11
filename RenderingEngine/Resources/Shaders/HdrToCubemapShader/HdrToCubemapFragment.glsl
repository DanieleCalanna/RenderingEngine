#version 400 core

out vec4 FragColor;
in vec3 WorldPos;

uniform sampler2D HdrMap;

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
	vec2 UV = SampleSphericalMap(normalize(WorldPos));
	FragColor = vec4(texture(HdrMap, UV).rgb, 1.0);
}