#version 400 core

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;

out vec2 UVFrag;
out vec3 FragPosition;
out vec3 FragTangent;
out vec3 FragBitangent;
out vec3 FragNormal;

uniform mat4 TransformationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
	vec4 Position_WS = TransformationMatrix * vec4(Position.xyz, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * Position_WS;
	FragPosition = Position_WS.xyz;

	FragTangent = Tangent;
	FragBitangent = Bitangent;
	FragNormal = Normal.xyz;

	UVFrag = UV;
}