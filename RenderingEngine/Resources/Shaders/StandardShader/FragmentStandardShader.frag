#version 400 core

#define Gamma 2.2
#define PI 3.1415926

in vec3 FragmentPosition_WS;
in vec3 LightDirection_TS;
in vec3 FragmentToCamera_TS;
in vec2 UVFrag;
in vec3 ReflectedVector;
in vec3 SurfaceNormal;

in vec3 FragTangent;
in vec3 FragBitangent;
in vec3 FragNormal;

out vec4 OutColor;

uniform vec3 BaseColor = vec3(0.2578, 0.5117, 0.95312);

uniform mat4 TransformationMatrix;
uniform mat4 ViewMatrix;

uniform vec3 LightColor;
uniform vec3 LightDirection;

uniform sampler2D AlbedoMap;
uniform sampler2D SpecularMap;
uniform sampler2D RoughnessMap;
uniform sampler2D NormalMap;
uniform sampler2D AOMap;

uniform samplerCube EnvironmentMap;

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

/*
vec3 Lambert(vec3 Albedo)
{
	return Albedo/PI;
}
*/

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float Denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    Denom        = PI * Denom * Denom;
	
    return a2 / Denom;
}

float GeometrySchlickGGX(float NdotV, float Roughness)
{
    return NdotV / (NdotV * (1.0 - Roughness) + Roughness);
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float Roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float GGX1 = GeometrySchlickGGX(NdotV, Roughness);
    float GGX2 = GeometrySchlickGGX(NdotL, Roughness);
	
    return GGX1 * GGX2;
}

void main()
{
	vec3 LightDirection_TS = normalize(LightDirection_TS);
	vec3 FragmentToCamera_TS = normalize(FragmentToCamera_TS);

	vec3 AlbedoColor = pow(texture(AlbedoMap, UVFrag).rgb, vec3(Gamma));
	vec3 Normal_TS = normalize(texture(NormalMap, UVFrag).rgb * (255.0/128.0) - 1.0);
	vec3 SpecularColor = texture(SpecularMap, UVFrag).rgb;
	vec3 RoughnessColor = texture(RoughnessMap, UVFrag).rgb;
	vec3 AOColor = texture(AOMap, UVFrag).rgb;

	/*TEMP*/
	mat3 TBN = transpose(mat3(
        normalize(cross(FragNormal, FragBitangent)),
        normalize(cross(FragTangent, FragNormal)),
        normalize(FragNormal)
    ));

	vec3 WorldNormal = normalize((TransformationMatrix*vec4(inverse(TBN)*Normal_TS, 0.0)).xyz);
	vec3 WorldCameraPosition = (inverse(ViewMatrix)*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 WorldReflectedVector = reflect(normalize(FragmentPosition_WS-WorldCameraPosition), WorldNormal);

	vec4 ReflectedColour = texture(EnvironmentMap, WorldReflectedVector);
	vec3 CameraToFragment = normalize(FragmentPosition_WS-WorldCameraPosition);
	/*TEMP*/

	vec3 HalfwayVector = normalize(FragmentToCamera_TS-LightDirection_TS);
	float D = DistributionGGX(Normal_TS, HalfwayVector, RoughnessColor.r);
	OutColor = vec4(D);

	float G = GeometrySmith(Normal_TS, FragmentToCamera_TS, -LightDirection_TS, RoughnessColor.r);
	OutColor = vec4(G);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, AlbedoColor, 1.0-SpecularColor.r);
	vec3 F = FresnelSchlick(dot(Normal_TS, FragmentToCamera_TS), F0);
	OutColor = vec4(F, 1.0);

	vec3 Numerator    = D * G * F;
	float Denominator = 4.0 * max(dot(Normal_TS, FragmentToCamera_TS), 0.0) * max(dot(Normal_TS, -LightDirection_TS), 0.0);
	vec3 Specular     = Numerator / max(Denominator, 0.001);  

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
  
	kD *= 1.0 - (1.0-SpecularColor.r);	

	vec3 Radiance = vec3(1.0, 1.0, 1.1)*LightIntensity;
	float NdotL = max(dot(Normal_TS, -LightDirection_TS), 0.0);        
    vec3 Lo = (kD * AlbedoColor / PI + Specular) * Radiance * NdotL;

	float AO = AOColor.r;
	vec3 Ambient = vec3(0.03) * AlbedoColor * AO;
    vec3 Color = Ambient + Lo;
	
    Color = Color / (Color + vec3(1.0));
   
    OutColor = vec4(Color, 1.0);
	OutColor.rgb = pow(OutColor.rgb, vec3(1.0/Gamma));
	
	//OutColor = vec4(WorldNormal, 1.0);
	//OutColor = vec4(AOColor, 1.0);
	//OutColor = vec4(ReflectedColour);
	//OutColor = vec4(pow(AlbedoColor,vec3(Gamma)), 1.0);
	
	//OutColor = vec4(max(dot(Normal_TS, -LightDirection_TS), 0.0));
	//OutColor.xyz = cross(FragTangent, FragNormal);
   	
}
