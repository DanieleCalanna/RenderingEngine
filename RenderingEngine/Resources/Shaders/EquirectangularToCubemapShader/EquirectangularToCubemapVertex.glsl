#version 400 core

layout(location = 0) in vec3 Position;

out vec3 Position_WS;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
	Position_WS = Position;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(Position_WS, 1.0);
}