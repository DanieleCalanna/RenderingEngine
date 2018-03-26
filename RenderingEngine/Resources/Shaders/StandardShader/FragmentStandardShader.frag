#version 400 core

in vec3 TangentLightDirection;
in vec3 toCameraVector;
in vec3 fromCameraVector;
in vec2 UVFrag;

out vec4 OutColor;

uniform vec3 BaseColor;
uniform float ShineDamper = 1.0f;
uniform float Reflectivity = 0.3f;

uniform vec3 LightColor;

uniform sampler2D Albedo;
uniform sampler2D Specular;
uniform sampler2D Roughness;
uniform sampler2D Normal;

#define LightIntensity 5 //TO-DO Make Param

void main()
{
	vec3 AlbedoColor = texture(Albedo, UVFrag).rgb;
	vec3 TangentNormal = normalize(texture(Normal, UVFrag).rgb * (255.0/128.0) - 1.0);
	//float specularFactor = dot(normal_tangent, unitVectorToCamera);
	//vec3 unitNormal = normalize(normalMapValue.xyz);
	//vec3 unitVectorToCamera = normalize(toCameraVector);

	//vec3 normals = normalize (TBN * normalMapColor.xyz) * vec3 (0.5) + vec3(0.5);
	float Diffuse = clamp(dot(TangentNormal, -TangentLightDirection*LightIntensity), 0.2, 1.0);
	vec3 DiffuseColor = AlbedoColor*Diffuse;
	OutColor = vec4(DiffuseColor, 1.0);
	//OutColor = vec4(AlbedoColor, 1.0);

	/*
	float nDot1 = dot(unitNormal, -unitLightVector);
	float brightness = max(nDot1, 0.15);
	vec4 diffuse = vec4(brightness * lightColor, 1.0) *texture(Albedo, uvFrag);

	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 reflectedLightDirection = reflect(unitLightVector, unitNormal);
	
	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0.0);
	float dampedFactor = pow(specularFactor, shineDamper);
	vec3 finalSpecular = dampedFactor * reflectivity * lightColor;
	out_Color = diffuse + vec4(finalSpecular, 1.0);
	*/

/*
	// FRESNEL
	const float bias = 0.5;
	const float scale = 2;
	const float power = 15;
	float fresnel = max(0.1, min(1, bias + scale * pow(1.0 + dot(fromCameraVector, surfaceNormal),power)));
	out_Color = vec4(fresnel, fresnel, fresnel, 1.0);
*/
	
}
