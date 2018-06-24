#version 400 core

#define Gamma 2.2
#define PI 3.1415926


in vec3 FragPosition;
in vec2 UVFrag;
in vec3 FragTangent;
in vec3 FragBitangent;
in vec3 FragNormal;

out vec4 OutColor;

uniform mat4 TransformationMatrix;
uniform mat4 ViewMatrix;

uniform vec3 LightColor;
uniform vec3 LightDirection;

uniform sampler2D AlbedoMap;
uniform sampler2D MetallicMap;
uniform sampler2D RoughnessMap;
uniform sampler2D NormalMap;
uniform sampler2D AOMap;

uniform samplerCube EnvironmentMap;
uniform samplerCube IrradianceMap;

uniform samplerCube PrefilterMap;
uniform sampler2D   BRDFLUT;

#define LightIntensity 2.0 //TO-DO Make Param

/*
float Map(float Value, float InMin, float InMax, float OutMin, float OutMax)
{
	return OutMin + ((Value-InMin)/(InMax-InMin)) * (OutMax-OutMin);
}
*/

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

float DistributionGGX(float NdotH, float Roughness)
{
	float Roughness2 = Roughness * Roughness;
	float NdotH2 = NdotH * NdotH;
	float Denom = (NdotH2 * (Roughness2 - 1.0) + 1.0);
	Denom = PI * Denom * Denom;
	return Roughness2 / Denom;
}

float GeometrySchlickGGX(float NdotV, float K)
{
	if (NdotV == 0.0) return 0.0;
	return NdotV * (1.0 / (NdotV * (1.0 - K) + K));
}

float GeometrySmith(float NdotV, float NdotL, float Roughness)
{
	float RoughnessPlus1 = Roughness + 1;
	float K = (RoughnessPlus1*RoughnessPlus1) / 8.0;
	float GGX1 = GeometrySchlickGGX(NdotV, K);
	float GGX2 = GeometrySchlickGGX(NdotL, K);
	return GGX1 * GGX2;
}

void main()
{
	vec3 AlbedoColor = pow(texture(AlbedoMap, UVFrag).rgb, vec3(Gamma));
	vec3 Normal_TS = normalize(texture(NormalMap, UVFrag).rgb * (255.0/128.0) - 1.0);
	float Metalness = texture(MetallicMap, UVFrag).r;
	Metalness = pow(Metalness, 2.2);
	float Roughness = texture(RoughnessMap, UVFrag).r;
	Roughness = pow(Roughness, 2.2);
	float AOFactor = texture(AOMap, UVFrag).r;

	mat3 TBN = transpose(mat3(
		normalize(cross(FragNormal, FragBitangent)),
		normalize(cross(FragTangent, FragNormal)),
		normalize(FragNormal)
	));

	vec3 CameraPosition_WS = (inverse(ViewMatrix)*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 N_WS = normalize((TransformationMatrix*vec4(inverse(TBN)*Normal_TS, 0.0)).xyz);
	vec3 V_WS = normalize(CameraPosition_WS - FragPosition);
	vec3 R_WS = reflect(normalize(FragPosition - CameraPosition_WS), N_WS);
	float NdotV = max(dot(N_WS, V_WS), 0.0);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, AlbedoColor, Metalness);

	vec3 Lo = vec3(0.0);
	{
		vec3 H = normalize(V_WS - LightDirection);
		float NdotL = max(dot(N_WS, -LightDirection), 0.0);
		float NdotH = max(dot(N_WS, H), 0.0);
		
		float D = DistributionGGX(NdotH, Roughness);
		float G = GeometrySmith(NdotV, NdotL, Roughness);
		vec3 F = FresnelSchlick(max(0.0, dot(H, V_WS)), F0);

		vec3 Numerator    = D * G * F;
		float Denominator = 4.0 * NdotV * NdotL;
		vec3 Specular     = Numerator * (1.0 / max(Denominator, 0.001));

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - (Metalness);

		vec3 Radiance = vec3(1.0, 1.0, 1.1)*LightIntensity;     
		Lo = (kD * AlbedoColor / PI + Specular) * Radiance * NdotL;
	}

	vec3 kS = FresnelSchlickRoughness(NdotV, F0, Roughness);
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - (Metalness);
	vec3 Irradiance = texture(IrradianceMap, N_WS).rgb;
	vec3 Diffuse = Irradiance * AlbedoColor;

	const float MAX_REFLECTION_LOD = 4.0;
	vec3 PrefilteredColor = textureLod(PrefilterMap, normalize(R_WS), Roughness * MAX_REFLECTION_LOD).rgb;

	vec2 EnvBDRF = texture(BRDFLUT, vec2(NdotV, Roughness)).rg;

	vec3 Specular = PrefilteredColor * (kS * EnvBDRF.x + EnvBDRF.y);

	vec3 Ambient = (kD * Diffuse + Specular) * AOFactor;

    vec3 Color = /*Lo +*/ Ambient;
	
    Color = Color / (Color + vec3(1.0));
	Color = pow(Color, vec3(1.0 / Gamma));
    OutColor = vec4(Color, 1.0);
}
