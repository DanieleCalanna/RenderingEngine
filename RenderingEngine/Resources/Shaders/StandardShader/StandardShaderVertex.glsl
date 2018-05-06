#version 400 core

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;

out vec2 UVFrag;
out vec3 ReflectedVector;
out vec3 FragmentPosition_WS;
out vec3 SurfaceNormal;
out vec3 FragTangent;
out vec3 FragBitangent;
out vec3 FragNormal;

out vec3 FragmentToCamera_TS;
out vec3 LightDirection_TS;

uniform mat4 TransformationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

uniform vec3 LightDirection;

void main()
{
	mat4 ModelViewMatrix = ViewMatrix * TransformationMatrix;
	vec4 Position_WS = TransformationMatrix * vec4(Position.xyz, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * Position_WS;
	FragmentPosition_WS = Position_WS.xyz;

	FragTangent = Tangent;
	FragBitangent = Bitangent;
	FragNormal = Normal.xyz;
	
	mat3 TBN = transpose(mat3(
        cross(Normal.xyz, Bitangent),
        cross(Tangent, Normal.xyz),
        Normal.xyz
    ));
	
	vec3 CameraPosition_WS = (inverse(ViewMatrix)*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 CameraForward_WS = (inverse(ViewMatrix)*vec4(0.0, 0.0, -1.0, 0.0)).xyz;

	LightDirection_TS = normalize((TBN * (inverse(TransformationMatrix) * vec4(LightDirection, 0.0)).xyz));
	FragmentToCamera_TS = normalize((TBN * (inverse(TransformationMatrix) * vec4((CameraPosition_WS-Position_WS.xyz), 0.0)).xyz));

	UVFrag = UV;
	SurfaceNormal = normalize((TransformationMatrix * Normal).xyz);

	ReflectedVector = reflect(normalize(Position_WS.xyz-CameraPosition_WS), normalize((TransformationMatrix*Normal).xyz));
}