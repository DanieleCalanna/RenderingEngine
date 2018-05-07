#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <map>

class Shader;

struct Character;

class GUI
{
private:
	Shader* TextShader;
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
	GUI();
public:
	void RenderText(std::string Text, GLfloat XPosition, GLfloat YPosition, GLfloat Scale, glm::vec3 Color);
	static GUI& GetInstance();
};