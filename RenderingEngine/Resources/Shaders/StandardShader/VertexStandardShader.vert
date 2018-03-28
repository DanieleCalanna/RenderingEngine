#version 400 core

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;

out vec3 TangentToCameraVector;
out vec2 UVFrag;
out vec3 TangentLightDirection;
out vec3 ReflectedVector;
out mat3 TBN;
out vec3 WorldFragPosition;

uniform mat4 TransformationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

uniform vec3 LightDirection;

void main()
{
	mat4 ModelViewMatrix = ViewMatrix * TransformationMatrix;
	vec4 WorldPosition = TransformationMatrix * Position;
	gl_Position = ProjectionMatrix * ViewMatrix * WorldPosition;
	WorldFragPosition = WorldPosition.xyz;

	TBN = transpose(mat3(
        Tangent,
        Bitangent,
        Normal.xyz
    ));

	vec3 WorldCameraPosition = (inverse(ViewMatrix)*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 WorldCameraForward = (inverse(ViewMatrix)*vec4(0.0, 0.0, -1.0, 0.0)).xyz;

	TangentLightDirection = normalize((TBN * (inverse(TransformationMatrix) * vec4(LightDirection, 0.0)).xyz));
	TangentToCameraVector = normalize((TBN * (inverse(TransformationMatrix) * vec4((WorldCameraPosition-WorldPosition.xyz), 0.0)).xyz));

	UVFrag = UV;

	ReflectedVector = reflect(normalize(WorldPosition.xyz-WorldCameraPosition), normalize((TransformationMatrix*Normal).xyz));
}