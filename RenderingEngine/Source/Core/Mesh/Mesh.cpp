#include <Core/Mesh/Mesh.hpp>

#include "Utils/Tga.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

class Tga;

std::vector<std::string> split2(const std::string& s, char delimiter) // TO-DO Move away
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


Mesh::Mesh(std::string ObjFilePath)
{
	LoadObj(ObjFilePath);
	InitBuffers();
}

void Mesh::InitBuffers()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	StoreDataInAttributeList(0, 4, &Vertices[0], (int)Vertices.size());
	StoreDataInAttributeList(1, 4, &Normals[0], (int)Normals.size());
	StoreDataInAttributeList(2, 2, &UVs[0], (int)UVs.size());
	glBindVertexArray(0);

	Tga info = Tga("D:/Download/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga");
	glGenerateMipmap(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, info.GetWidth(), info.GetWidth(), 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, info.GetPixels().data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::LoadObj(std::string ObjFilePath)
{
	Clear();

	std::vector< glm::vec4 > ObjVertices;
	std::vector< glm::vec2 > ObjUVs;
	std::vector< glm::vec4 > ObjNormals;
	std::vector< unsigned int > ObjVerticesIndices, ObjUVsIndices, ObjNormalsIndices;

	//Open the Obj fileDom
	ifstream ObjFile(ObjFilePath);
	if (!ObjFile.is_open())
	{
		std::cout<<"Impossible to open the file !"<<std::endl;
		return;
	}

	string LineOfObjFile;
	while (getline(ObjFile, LineOfObjFile)) // for each line in the Obj file
	{
		std::vector<std::string> WordsInLine = split2(LineOfObjFile, ' ');

		if (WordsInLine[0] == "v")
		{
			glm::vec4 Vertex;
			Vertex.x = stof(WordsInLine[1]);
			Vertex.y = stof(WordsInLine[2]);
			Vertex.z = stof(WordsInLine[3]);
			Vertex.w = 0.0f;
			ObjVertices.push_back(Vertex);
		}
		else if (WordsInLine[0] == "vt")
		{
			glm::vec2 UV;
			UV.x = stof(WordsInLine[1]);
			UV.y = stof(WordsInLine[2]);
			ObjUVs.push_back(UV);
		}
		else if (WordsInLine[0] == "vn")
		{
			glm::vec4 Normal;
			Normal.x = stof(WordsInLine[1]);
			Normal.y = stof(WordsInLine[2]);
			Normal.z = stof(WordsInLine[3]);
			Normal.w = 0.0f;
			ObjNormals.push_back(Normal);
		}
		else if (WordsInLine[0] == "f")
		{
			std::vector<std::string> ParamsOfVertex0 = split2(WordsInLine[1], '/');
			std::vector<std::string> ParamsOfVertex1 = split2(WordsInLine[2], '/');
			std::vector<std::string> ParamsOfVertex2 = split2(WordsInLine[3], '/');

			unsigned int FaceVerticesIndices[3], FaceUVsIndices[3], FaceNormalsIndices[3];

			// Read params from the line
			FaceVerticesIndices[0] = stoi(ParamsOfVertex0[0]);
			FaceUVsIndices[0] = stoi(ParamsOfVertex0[1]);
			FaceNormalsIndices[0] = stoi(ParamsOfVertex0[2]);

			FaceVerticesIndices[1] = stoi(ParamsOfVertex1[0]);
			FaceUVsIndices[1] = stoi(ParamsOfVertex1[1]);
			FaceNormalsIndices[1] = stoi(ParamsOfVertex1[2]);

			FaceVerticesIndices[2] = stoi(ParamsOfVertex2[0]);
			FaceUVsIndices[2] = stoi(ParamsOfVertex2[1]);
			FaceNormalsIndices[2] = stoi(ParamsOfVertex2[2]);

			//Put params in the 
			ObjVerticesIndices.push_back(FaceVerticesIndices[0] -1);
			ObjVerticesIndices.push_back(FaceVerticesIndices[1] -1);
			ObjVerticesIndices.push_back(FaceVerticesIndices[2] -1);
			ObjUVsIndices.push_back(FaceUVsIndices[0] -1);
			ObjUVsIndices.push_back(FaceUVsIndices[1] -1);
			ObjUVsIndices.push_back(FaceUVsIndices[2] -1);
			ObjNormalsIndices.push_back(FaceNormalsIndices[0] -1);
			ObjNormalsIndices.push_back(FaceNormalsIndices[1] -1);
			ObjNormalsIndices.push_back(FaceNormalsIndices[2] -1);
		}
	}
	// For each vertex of each triangle
	for(unsigned int i=0; i< (unsigned int) ObjVerticesIndices.size(); i++) 
	{
		unsigned int VertexIndex = ObjVerticesIndices[i];
		unsigned int UVIndex = ObjUVsIndices[i];
		unsigned int NormalIndex = ObjNormalsIndices[i];
		Vertices.push_back(ObjVertices[VertexIndex]);
		UVs.push_back(ObjUVs[UVIndex]);
		Normals.push_back(ObjNormals[NormalIndex]);
	}
}

void Mesh::Render()
{
	glBindVertexArray(VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());

	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

GLuint Mesh::StoreDataInAttributeList(unsigned int AttributeIndex, unsigned int CoordinateSize, GLvoid* Data, int NumberOfElements)
{
	GLuint VertexBufferID; 
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, NumberOfElements * CoordinateSize * sizeof(GLfloat), Data, GL_STATIC_DRAW);
	glVertexAttribPointer(AttributeIndex, CoordinateSize, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return VertexBufferID;
}


void Mesh::Clear()
{
	Vertices.clear();
	UVs.clear();
	Normals.clear();
	//TO-DO
}
