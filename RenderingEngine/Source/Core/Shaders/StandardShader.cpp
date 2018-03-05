#include <Core/Shaders/StandardShader.hpp>

StandardShader::StandardShader(){
	vertexShaderID = createShaderFromFile("res/Shaders/StandardShader/vertexShader.vert", GL_VERTEX_SHADER);
	fragmentShaderID = createShaderFromFile("res/Shaders/StandardShader/fragmentShader.frag", GL_FRAGMENT_SHADER);
	programID = createProgram(vertexShaderID, fragmentShaderID);
	glValidateProgram(programID);
	
	start();

	/*Temp TO-DO*/
	glm::vec3 tempColor(0.2578f, 0.5117f, 0.95312f);
	loadColor(tempColor);
	//glUniform3fv(glGetUniformLocation(programID, "color"), 1, &tempColor[0]);
	glUniform1f(glGetUniformLocation(programID, "shineDamper"), 1.0f);
	glUniform1f(glGetUniformLocation(programID, "reflectivity"), 0.4f);
	stop();
	/*Temp TO-DO End*/
}

void StandardShader::start(){
	glUseProgram(programID);
}
void StandardShader::stop(){
	glUseProgram(0);
}

void StandardShader::loadColor(const glm::vec3 &color){
	glUniform3fv(glGetUniformLocation(programID, "color"), 1, &color[0]);
}

GLuint StandardShader::createShaderFromFile(std::string path, GLenum shaderType){
	std::string shaderSource = loadFile(path);
	//std::cout<<"Shader loaded: "<<path<<std::endl<<shaderSource<<std::endl;
	const GLuint shaderID = glCreateShader(shaderType);
	const GLchar *source = (const GLchar *)shaderSource.c_str();
	glShaderSource(shaderID, 1, &source, 0);
	glCompileShader(shaderID);

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	
	if(isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength); //Get Log Length
		
		char *infoLog = new char[maxLength];
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, infoLog);
		glDeleteShader(shaderID);
		std::cout<<"COMPILING SHADER ERROR: "<<path<<std::endl<<infoLog<<std::endl;
		exit(1);
	}
	return shaderID;
}

GLuint StandardShader::createProgram(GLuint vertexShaderID, GLuint fragmentShaderID){
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glBindAttribLocation(programID, 0, "position");
	glBindAttribLocation(programID, 1, "normal");
	glLinkProgram(programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		GLchar* infoLog = new GLchar[maxLength];
		glGetProgramInfoLog(programID, maxLength, &maxLength, infoLog);
		
		//We don't need the program anymore.
		glDeleteProgram(programID);
		//Don't leak shaders either.
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		std::cout<<"LINKING PROGRAM ERROR: "<<std::endl<<infoLog<<std::endl;
		delete[] infoLog;
		exit(1);
	}
	//Always detach shaders after a successful link.
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	return programID;
}

void StandardShader::loadTransformationMatrix(glm::mat4x4 transformationMatrix){
	GLint location = glGetUniformLocation(programID, "transformationMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &transformationMatrix[0][0]);
}

void StandardShader::loadProjectionMatrix(glm::mat4x4 projectionMatrix){
	GLint location = glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void StandardShader::loadViewMatrix(glm::mat4x4 viewMatrix){
	GLint location = glGetUniformLocation(programID, "viewMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &viewMatrix[0][0]);
}


void StandardShader::loadDirectionalLight(DirectionalLight directionalLight){
	GLint location = glGetUniformLocation(programID, "lightColor");
	glUniform3fv(location, 1, &directionalLight.color[0]);
	location = glGetUniformLocation(programID, "lightDirection");
	glUniform3fv(location, 1, &directionalLight.direction[0]);
}






std::string StandardShader::loadFile(std::string path){
	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return str;
}