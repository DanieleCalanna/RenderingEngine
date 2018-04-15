#include "Core/Mesh/Mesh.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <glm/glm.hpp>

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <list>

using namespace std;

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
	StoreDataInAttributeList(3, 3, &Tangents[0], (int)Tangents.size());
	StoreDataInAttributeList(4, 3, &Bitangents[0], (int)Bitangents.size());
	glBindVertexArray(0);
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
			Vertex.w = 1.0f;
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
			ObjNormals.push_back(glm::normalize(Normal));
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
	std::list<unsigned int>* A = new std::list<unsigned int>[ObjVertices.size()];
	// For each vertex of each triangle
	for(unsigned int i=0; i< (unsigned int) ObjVerticesIndices.size(); i++) 
	{
		unsigned int VertexIndex = ObjVerticesIndices[i];
		unsigned int UVIndex = ObjUVsIndices[i];
		unsigned int NormalIndex = ObjNormalsIndices[i];
		A[VertexIndex].push_back((unsigned int)Vertices.size());
		Vertices.push_back(ObjVertices[VertexIndex]);
		UVs.push_back(ObjUVs[UVIndex]);
		Normals.push_back(ObjNormals[NormalIndex]);
	}

	std::vector	< glm::vec3 > FlatTangents;
	std::vector	< glm::vec3 > FlatBitangents;
	for (int i = 0; i < Vertices.size(); i += 3)
	{

		// Shortcuts for vertices
		glm::vec3 v0 = Vertices[i + 0];
		glm::vec3 v1 = Vertices[i + 1];
		glm::vec3 v2 = Vertices[i + 2];

		// Shortcuts for UVs
		glm::vec2 & uv0 = UVs[i + 0];
		glm::vec2 & uv1 = UVs[i + 1];
		glm::vec2 & uv2 = UVs[i + 2];

		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 Tangent = glm::normalize((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r);
		glm::vec3 Bitangent = glm::normalize((deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r);

		// Set the same tangent for all three vertices of the triangle.
		// They will be merged later, in vboindexer.cpp
		FlatTangents.push_back(Tangent);
		FlatTangents.push_back(Tangent);
		FlatTangents.push_back(Tangent);

		Tangents.push_back(Tangent);
		Tangents.push_back(Tangent);
		Tangents.push_back(Tangent);

		// Same thing for bitangents
		FlatBitangents.push_back(Bitangent);
		FlatBitangents.push_back(Bitangent);
		FlatBitangents.push_back(Bitangent);

		Bitangents.push_back(Bitangent);
		Bitangents.push_back(Bitangent);
		Bitangents.push_back(Bitangent);
	}
	/*
	for (unsigned int i = 0; i< (unsigned int)ObjVertices.size(); i++)
	{
		glm::vec3 SmoothTangent(0.0f);
		glm::vec3 SmoothBitangen(0.0f);
		std::list<unsigned int> AA = A[i];
		for (unsigned int VertexIndex : AA)
		{
			SmoothTangent += FlatTangents[VertexIndex];
			SmoothBitangen += FlatBitangents[VertexIndex];
		}
		SmoothTangent /= AA.size();
		SmoothBitangen /= AA.size();
		for (unsigned int VertexIndex : AA)
		{
			Tangents[VertexIndex] = SmoothTangent;
			Bitangents[VertexIndex] = SmoothBitangen;
		}
	}
	delete[] A;
	*/
}

void Mesh::Render()
{
	glBindVertexArray(VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
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
