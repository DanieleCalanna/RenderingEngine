#version 400

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 UV;

uniform mat4 TransformationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec3 FragPosition;
out vec3 FragNormal;
out vec3 CameraPosition;
out vec3 ViewVector;
out vec2 UVFrag;

void main()
{
	UVFrag = UV;
	CameraPosition = (inverse(ViewMatrix)*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	ViewVector = normalize((inverse(ViewMatrix)*vec4(0.0, 0.0, -1.0, 0.0)).xyz);
	FragNormal = normalize( (TransformationMatrix * vec4(Normal.xyz, 0.0)).xyz );
	FragPosition = (TransformationMatrix * vec4(Position.xyz, 1.0)).xyz;

	gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPosition, 1.0);
}