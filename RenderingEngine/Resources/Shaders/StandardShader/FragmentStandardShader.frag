#version 150

in vec3 surfaceNormal;
in vec3 toCameraVector;
in vec3 fromCameraVector;

out vec4 out_Color;

uniform vec3 color;
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform float shineDamper = 1.0f;
uniform float reflectivity = 0.3f;


void main()
{
	vec3 unitNormal = normalize(surfaceNormal); 
	vec3 unitLightVector = normalize(lightDirection); 

	float nDot1 = dot(unitNormal, -unitLightVector);
	float brightness = max(nDot1, 0.25);
	vec3 diffuse = brightness * lightColor;
	
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 reflectedLightDirection = reflect(unitLightVector, unitNormal);
	
	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0.0);
	float dampedFactor = pow(specularFactor, shineDamper);
	vec3 finalSpecular = dampedFactor * reflectivity * lightColor;
	out_Color = vec4(diffuse, 1.0) * vec4(color,1.0) + vec4(finalSpecular,1.0);
	
	//out_Color = vec4(surfaceNormal, 1.0);

/*
	// FRESNEL
	const float bias = 0.5;
	const float scale = 2;
	const float power = 15;
	float fresnel = max(0.1, min(1, bias + scale * pow(1.0 + dot(fromCameraVector, surfaceNormal),power)));
	out_Color = vec4(fresnel, fresnel, fresnel, 1.0);
*/

	//out_Color = vec4(color,1.0);

	
}
