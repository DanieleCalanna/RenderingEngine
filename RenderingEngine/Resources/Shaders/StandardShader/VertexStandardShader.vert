#version 400 core

layout(location = 0) in vec4 vertexPosition_model;
layout(location = 1) in vec4 vertexNormal_model;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTangent_model;
layout(location = 4) in vec3 vertexBitangent_model;

out vec3 fromCameraVector;
out vec3 toCameraVector;
out vec2 UV;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightDirection;

void main()
{
	mat4 ModelViewMatrix = viewMatrix * transformationMatrix;
	vec4 vertexPosition_world = transformationMatrix * vec4(vertexPosition_model.xyz, 1.0);
	gl_Position = projectionMatrix * viewMatrix * vertexPosition_world;

	vec3 vertexNormal_camera = (ModelViewMatrix * normalize(vertexNormal_model)).xyz;
    vec3 vertexTangent_camera = (ModelViewMatrix * vec4(normalize(vertexTangent_model), 1.0)).xyz;
    vec3 vertexBitangent_camera = (ModelViewMatrix * vec4(normalize(vertexBitangent_model), 1.0)).xyz;
	
	mat3 TBN = transpose(mat3(
        vertexTangent_camera,
        vertexBitangent_camera,
        vertexNormal_camera
    ));
	
	toCameraVector = normalize(TBN * ((inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - vertexPosition_world.xyz));
	fromCameraVector = normalize(vertexPosition_world.xyz - (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz);

	vec3 LightDirection_tangentspace = TBN * (viewMatrix*vec4(lightDirection, 1.0)).xyz;
    //vec3 EyeDirection_tangentspace =  TBN * lightDirection;
	UV = vertexUV;
}