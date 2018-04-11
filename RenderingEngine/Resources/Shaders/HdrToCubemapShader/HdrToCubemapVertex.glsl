#version 400 core

layout(location = 0) in vec3 Position;

out vec3 WorldPos;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	WorldPos = Position;
	gl_Position = Projection * View * vec4(WorldPos, 1.0);
}