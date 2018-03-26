#version 400 core

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;

out vec3 fromCameraVector;
out vec3 toCameraVector;
out vec2 UVFrag;
out vec3 TangentLightDirection;

uniform mat4 TransformationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

uniform vec3 LightDirection;

void main()
{
	mat4 ModelViewMatrix = ViewMatrix * TransformationMatrix;
	vec4 WorldPosition = TransformationMatrix * Position;
	gl_Position = ProjectionMatrix * ViewMatrix * WorldPosition;

	vec3 CameraNormal = normalize((normalize(Normal)).xyz);
    vec3 CameraTangent = normalize((vec4(normalize(Tangent), 0.0)).xyz);
    vec3 CameraBitangent = normalize((vec4(normalize(Bitangent), 0.0)).xyz);
	
	mat3 TBN = transpose(mat3(
        CameraTangent,
        CameraBitangent,
        CameraNormal
    ));

	TangentLightDirection = normalize((TBN * (inverse(TransformationMatrix) * vec4(LightDirection, 0.0)).xyz));
	
	//toCameraVector = normalize(TBN * ((inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - vertexPosition_world.xyz));
	//fromCameraVector = normalize(vertexPosition_world.xyz - (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz);

	//vec3 LightDirection_tangentspace = TBN * (viewMatrix*vec4(lightDirection, 1.0)).xyz;
    //vec3 EyeDirection_tangentspace =  TBN * lightDirection;
	UVFrag = UV;
}