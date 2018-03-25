#version 400 core

in vec3 toCameraVector;
in vec3 fromCameraVector;
in vec2 UV;

out vec4 out_Color;

uniform vec3 color;
uniform vec3 lightColor;
uniform float shineDamper = 1.0f;
uniform float reflectivity = 0.3f;

uniform sampler2D Albedo;
uniform sampler2D Specular;
uniform sampler2D Roughness;
uniform sampler2D Normal;

void main()
{
	vec3 normal_tangent = normalize(texture(Normal, UV).rgb * (255.0/128.0) - 1.0);
	//float specularFactor = dot(normal_tangent, unitVectorToCamera);
	//vec3 unitNormal = normalize(normalMapValue.xyz);
	//vec3 unitVectorToCamera = normalize(toCameraVector);

	//vec3 normals = normalize (TBN * normalMapColor.xyz) * vec3 (0.5) + vec3(0.5);

	out_Color = vec4(toCameraVector.xyz, 1.0);

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
