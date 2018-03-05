#include <Core/Mesh/Mesh.hpp>

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

std::vector<std::string> split2(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

Mesh::Mesh(std::vector < glm::vec4 >vertices, std::vector < glm::vec4 >normals){
	this->vertices = vertices;
	this->normals = normals;
	InitBuffers();
}
void Mesh::InitBuffers(){
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	storeDataInAttributeList(0, 4, &vertices[0], vertices.size());
	storeDataInAttributeList(1, 4, &normals[0], normals.size());
	glBindVertexArray(0);
}

Mesh* Mesh::FromOBJFile(std::string path){

	Mesh* mesh = new Mesh();

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec4 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;

	ifstream file(path);
	if (!file.is_open())
	{
		printf("Impossible to open the file !\n");
		return mesh;
	}
	string line;
	while (getline(file, line))
	{

		std::vector<std::string> words = split2(line, ' ');

		// else : parse lineHeader
		if (words[0] == "v")
		{
			glm::vec4 vertex;
			vertex.x = stof(words[1]);
			vertex.y = stof(words[2]);
			vertex.z = stof(words[3]);
			vertex.w = 0.0f;
			mesh->vertices.push_back(vertex);
			temp_vertices.push_back(vertex);
		}
		else if (words[0] == "vt")
		{
			glm::vec2 uv;
			uv.x = stof(words[1]);
			uv.y = stof(words[2]);
			temp_uvs.push_back(uv);
		}
		else if (words[0] == "vn")
		{
			glm::vec4 normal;
			normal.x = stof(words[1]);
			normal.y = stof(words[2]);
			normal.z = stof(words[3]);
			normal.w = 0.0f;
			temp_normals.push_back(normal);
		}
		else if (words[0] == "f")
		{
			std::vector<std::string> ParamsOfVertex0 = split2(words[1], '/');
			std::vector<std::string> ParamsOfVertex1 = split2(words[2], '/');
			std::vector<std::string> ParamsOfVertex2 = split2(words[3], '/');

			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			vertexIndex[0] = stoi(ParamsOfVertex0[0]);
			uvIndex[0] = stoi(ParamsOfVertex0[1]);
			normalIndex[0] = stoi(ParamsOfVertex0[2]);

			vertexIndex[1] = stoi(ParamsOfVertex1[0]);
			uvIndex[1] = stoi(ParamsOfVertex1[1]);
			normalIndex[1] = stoi(ParamsOfVertex1[2]);

			vertexIndex[2] = stoi(ParamsOfVertex2[0]);
			uvIndex[2] = stoi(ParamsOfVertex2[1]);
			normalIndex[2] = stoi(ParamsOfVertex2[2]);
			vertexIndices.push_back(vertexIndex[0] -1);
			vertexIndices.push_back(vertexIndex[1] -1);
			vertexIndices.push_back(vertexIndex[2] -1);
			uvIndices    .push_back(uvIndex[0] -1);
			uvIndices    .push_back(uvIndex[1] -1);
			uvIndices    .push_back(uvIndex[2] -1);
			normalIndices.push_back(normalIndex[0] -1);
			normalIndices.push_back(normalIndex[1] -1);
			normalIndices.push_back(normalIndex[2] -1);
		}
	}
	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int normalIndex = normalIndices[i];
		mesh->vertices.push_back(temp_vertices[ vertexIndex ]);
		mesh->normals.push_back(temp_normals[ normalIndex ]);
	}
	mesh->InitBuffers();
	return mesh;
}

void Mesh::Render(){

	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

}

GLuint Mesh::storeDataInAttributeList(int attributeNumber, int coordinateSize, GLvoid * data, int size){
	GLuint vboID; 
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, size * coordinateSize * sizeof(GLfloat), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vboID;
}


void Mesh::Clear(){
	//TO-DO
}