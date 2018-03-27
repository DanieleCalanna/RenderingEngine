#version 400 core

in vec3 TangentLightDirection;
in vec3 TangentToCameraVector;
in vec3 fromCameraVector;
in vec2 UVFrag;
in vec3 ReflectedVector;

out vec4 OutColor;

uniform vec3 BaseColor;
uniform float ShineDamper = 1.0f;
uniform float Reflectivity = 0.3f;

uniform vec3 LightColor;

uniform sampler2D Albedo;
uniform sampler2D Specular;
uniform sampler2D Roughness;
uniform sampler2D Normal;

uniform samplerCube EnvironmentMap;

#define LightIntensity 1 //TO-DO Make Param

float Map(float Value, float InMin, float InMax, float OutMin, float OutMax)
{
	return OutMin + ((Value-InMin)/(InMax-InMin)) * (OutMax-OutMin);
}

void main()
{
	vec3 AlbedoColor = texture(Albedo, UVFrag).rgb;
	vec3 TangentNormal = normalize(texture(Normal, UVFrag).rgb * (255.0/128.0) - 1.0);

	float Diffuse = clamp(dot(TangentNormal, -TangentLightDirection), 0.00001, 1.0) * LightIntensity;
	vec3 DiffuseColor = AlbedoColor* Diffuse;

	vec3 SpecularColor = texture(Specular, UVFrag).rgb;
	float Specular = max(dot(reflect(TangentLightDirection, TangentNormal), TangentToCameraVector),0)*SpecularColor.r;

	OutColor = vec4(DiffuseColor, 1.0);
	OutColor = vec4(DiffuseColor+vec3(Specular*0.1), 1.0);
	//OutColor = vec4(Specular*0.1,Specular*0.1,Specular*0.1, 1.0);
	//vec3 finalSpecular = dampedFactor * reflectivity * lightColor;

	vec4 ReflectedColour = texture(EnvironmentMap, ReflectedVector);
	OutColor = ReflectedColour;
/*
	// FRESNEL
	const float Bias = 0.2;
	const float Scale = 0.0001;
	const float Power = 10;
	float fresnel = max(0.1, min(1, Bias + Scale * pow(1.0 + max(0, dot(TangentNormal, TangentToCameraVector)), Power)));
	OutColor = vec4(fresnel, fresnel, fresnel, 1.0);
*/
	
}
