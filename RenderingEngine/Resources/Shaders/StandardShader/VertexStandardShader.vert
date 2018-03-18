#version 400 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uv;

out vec3 surfaceNormal;
out vec3 toCameraVector;
out vec3 fromCameraVector;
out vec2 uvFrag;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{

	vec4 worldPosition = transformationMatrix * vec4(position.xyz, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
	
	surfaceNormal = (transformationMatrix * normal).xyz;
	
	toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;
	fromCameraVector = normalize(worldPosition.xyz - (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz);

	uvFrag = uv;
}