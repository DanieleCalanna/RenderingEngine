#version 400

layout(location = 0) in vec3 Position;

out vec3 FragPosition;

void main()
{
	FragPosition = Position;
	gl_Position = vec4(Position, 1.0);
}