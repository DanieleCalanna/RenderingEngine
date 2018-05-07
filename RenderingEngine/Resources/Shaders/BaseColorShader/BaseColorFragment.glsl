#version 400

#define PI 3.1415926

/*
* CosTheta is the angle between the direction from which the incident light is coming
* and the normal of the interface between the two media.
* F0 is the reflection coefficient for light incoming parallel to the normal.
*/
vec3 FresnelSchlick(float CosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - CosTheta, 5.0);
}

vec3 FresnelSchlickRoughness(float CosTheta, vec3 F0, float Roughness)
{
	return F0 + (max(vec3(1.0 - Roughness), F0) - F0) * pow(1.0 - CosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float Roughness)
{
	float Roughness2 = Roughness * Roughness;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float Denom = (NdotH2 * (Roughness2 - 1.0) + 1.0);
	Denom = PI * Denom * Denom;

	return Roughness2 / Denom;
}

float GeometrySchlickGGX(float NdotV, float Roughness)
{
	if (NdotV == 0.0) return 0.0;
	return NdotV * (NdotV * (1.0 - Roughness) + Roughness);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float Roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float GGX1 = GeometrySchlickGGX(NdotV, Roughness);
	float GGX2 = GeometrySchlickGGX(NdotL, Roughness);

	return GGX1 * GGX2;
}

uniform mat4 TransformationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform samplerCube IrradianceMap;

in vec3 FragPosition;
in vec3 FragNormal;
in vec3 CameraPosition;
in vec3 ViewVector;
in vec2 UVFrag;
out vec4 FragColor;

uniform vec3 LightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 LightDirection = vec3(-4.5, -4.5, -4.5);
uniform float LightIntensity = 1.0;
uniform vec3 BaseColor = vec3(1.0, 0.0, 0.0);
uniform float Roughness = 0.1;
uniform float Metalness = 1.0;

void main()
{
	vec3 NormalLightDirection = normalize(LightDirection);
	vec3 SurfaceNormal = normalize(FragNormal);

	vec3 FragmentToCamera = normalize(CameraPosition - FragPosition);
	vec3 HalfwayVector = normalize(FragmentToCamera - NormalLightDirection);
	
	float NDF = DistributionGGX(SurfaceNormal, HalfwayVector, Roughness);

	float G = GeometrySmith(SurfaceNormal, FragmentToCamera, -NormalLightDirection, Roughness);

	vec3 F0 = mix(vec3(0.04), BaseColor, Metalness);
	vec3 F = FresnelSchlick(max(0.0, dot(HalfwayVector, FragmentToCamera)), F0);

	vec3 Numerator = NDF * G * F;
	float Denominator = 4.0 * max(dot(SurfaceNormal, FragmentToCamera), 0.0) * max(dot(SurfaceNormal, -NormalLightDirection), 0.0);
	vec3 Specular = Numerator* (1.0 / max(Denominator, 0.001));

	vec3 LightKS = F; //Specular factor

	vec3 LightKD = vec3(1.0) - LightKS; //Diffuse factor
	LightKD *= 1.0 - Metalness;
	
	vec3 Lo = (LightKD * BaseColor * (1.0f / PI) + Specular)* LightColor * LightIntensity * max(0.0, dot(SurfaceNormal, -NormalLightDirection));

	vec3 IrradianceKS = FresnelSchlickRoughness(max(0.0, dot(SurfaceNormal, FragmentToCamera)), F0, Roughness);
	vec3 IrradianceKD = 1.0 - IrradianceKS;
	vec3 Irradiance = texture(IrradianceMap, SurfaceNormal).rgb;
	vec3 Diffuse = Irradiance * BaseColor;
	vec3 Ambient = (IrradianceKD * Diffuse);

	vec3 Shade = Lo/* + Ambient*/;

	FragColor = vec4(Shade, 1.0);

	FragColor.xyz = FragColor.xyz / (FragColor.xyz + vec3(1.0));
	FragColor.xyz = pow(FragColor.xyz, vec3(1.0 / 2.2));
}