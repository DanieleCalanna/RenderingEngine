#version 400

layout(location = 0) in vec3 Position;

out vec3 FragPosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
	FragPosition = Position;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(Position, 1.0);
}