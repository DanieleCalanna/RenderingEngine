#version 400 core

in vec3 WorldFragPosition;
in vec3 TangentLightDirection;
in vec3 TangentToCameraVector;
in vec2 UVFrag;
in vec3 ReflectedVector;
in mat3 TBN;

uniform mat4 TransformationMatrix;
uniform mat4 ViewMatrix;

out vec4 OutColor;

uniform vec3 BaseColor;
uniform float ShineDamper = 1.0f;
uniform float Reflectivity = 0.3f;

uniform vec3 LightColor;

uniform sampler2D AlbedoMap;
uniform sampler2D SpecularMap;
uniform sampler2D RoughnessMap;
uniform sampler2D NormalMap;

uniform samplerCube EnvironmentMap;

#define LightIntensity 1 //TO-DO Make Param

float Map(float Value, float InMin, float InMax, float OutMin, float OutMax)
{
	return OutMin + ((Value-InMin)/(InMax-InMin)) * (OutMax-OutMin);
}

void main()
{

	vec3 AlbedoColor = texture(AlbedoMap, UVFrag).rgb;
	vec3 TangentNormal = normalize(texture(NormalMap, UVFrag).rgb * (255.0/128.0) - 1.0);

	float DiffuseFactor = clamp(dot(TangentNormal, -TangentLightDirection), 0.00001, 1.0) * LightIntensity;
	vec3 Diffuse = AlbedoColor* DiffuseFactor;

	vec3 SpecularColor = texture(SpecularMap, UVFrag).rgb;
	float SpecularFactor = max(dot(reflect(TangentLightDirection, TangentNormal), TangentToCameraVector),0)*SpecularColor.r;
	vec3 Specular = vec3(SpecularFactor*0.1);

	vec3 WorldNormal = normalize((TransformationMatrix*vec4(inverse(TBN)*TangentNormal, 0.0)).xyz);
	vec3 WorldCameraPosition = (inverse(ViewMatrix)*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 WorldReflectedVector = reflect(normalize(WorldFragPosition-WorldCameraPosition), WorldNormal);

	vec3 RoughnessColor = normalize(texture(RoughnessMap, UVFrag).rgb * (255.0/128.0) - 1.0);
	//RoughnessColor = normalize(texture(RoughnessMap, UVFrag).rgb);
	vec4 ReflectedColour = texture(EnvironmentMap, WorldReflectedVector);

	// FRESNEL
	const float Bias = 0.001;
	const float Scale = 0.06;
	const float Power = 0.8;
	float Fresnel = max(0.1, min(1, Bias + Scale * pow(1.0 + max(0, 0.5/abs(dot(normalize(-TangentToCameraVector), TangentNormal))), Power)));
	OutColor = vec4(Fresnel, Fresnel, Fresnel, 1.0);
	OutColor = mix(ReflectedColour, vec4(Diffuse+Specular, 1.0), clamp(Map(RoughnessColor.r, 0.0, 1.0, 0.4232, 1.0)/Fresnel, 0.0, 1.0));
	//OutColor = vec4(RoughnessColor,1.0);
	//vec3 finalSpecular = dampedFactor * reflectivity * lightColor;
	//OutColor = vec4(RoughnessColor, 1.0);
}
