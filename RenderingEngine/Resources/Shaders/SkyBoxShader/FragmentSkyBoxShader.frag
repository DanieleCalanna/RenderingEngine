#version 400

out vec4 out_Color;

in vec3 texcoords;

uniform samplerCube cube_texture;

void main()
{
	out_Color = texture(cube_texture, texcoords);
}
