#include "Core/Mesh/IndexedMesh.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

std::vector<std::string> split(const std::string& s, char delimiter) //TO-DO Move away
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

void IndexedMesh::InitBuffers()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexBuffer = storeDataInAttributeListShared(0, 4, &Vertices[0], (int)Vertices.size());
	cout<<"vertexBuffer created"<<endl;
	normalsBuffer = storeDataInAttributeListShared(1, 4, &Normals[0], (int)Normals.size());
	glBindVertexArray(0);
}

Mesh* IndexedMesh::FromOBJFile(std::string path)
{

	IndexedMesh *indexedMesh = new IndexedMesh();
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec4 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;

	ifstream file(path);
	if (!file.is_open())
	{
		printf("Impossible to open the file !\n");
		return indexedMesh;
	}
	string line;
	while (getline(file, line))
	{

		std::vector<std::string> words = split(line, ' ');

		// else : parse lineHeader
		if (words[0] == "v")
		{
			glm::vec4 vertex;
			vertex.x = stof(words[1]);
			vertex.y = stof(words[2]);
			vertex.z = stof(words[3]);
			vertex.w = 0.0f;
			indexedMesh->Vertices.push_back(vertex);
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
			std::vector<std::string> ParamsOfVertex0 = split(words[1], '/');
			std::vector<std::string> ParamsOfVertex1 = split(words[2], '/');
			std::vector<std::string> ParamsOfVertex2 = split(words[3], '/');

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

			indexedMesh->indices.push_back(vertexIndex[0] - 1);
			indexedMesh->indices.push_back(vertexIndex[1] - 1);
			indexedMesh->indices.push_back(vertexIndex[2] - 1);
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	glm::vec4 *Normals = new glm::vec4[indexedMesh->Vertices.size()];
	// For each vertex of each triangle

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i] - 1;
		unsigned int normalIndex = normalIndices[i] - 1;
		Normals[vertexIndex] = temp_normals[normalIndex];
	}
	for (int i = 0; i < (int)indexedMesh->Vertices.size(); i++)
	{
		indexedMesh->Normals.push_back(Normals[i]);
	}


	// Init number of vertex
	indexedMesh->nels = (int)indexedMesh->Vertices.size();
	// Discover adjacents vertex for each vertex
	std::vector< unsigned int >* adjacents = new std::vector< unsigned int >[indexedMesh->nels];

	for (int i = 0; i < vertexIndices.size(); i += 3)
	{
		unsigned int vertexID1 = vertexIndices[i] - 1;
		unsigned int vertexID2 = vertexIndices[i + 1] - 1;
		unsigned int vertexID3 = vertexIndices[i + 2] - 1;

		std::vector< unsigned int >* adjacent1 = &adjacents[vertexID1];
		std::vector< unsigned int >* adjacent2 = &adjacents[vertexID2];
		std::vector< unsigned int >* adjacent3 = &adjacents[vertexID3];

		if (std::find(adjacent1->begin(), adjacent1->end(), vertexID2) == adjacent1->end())
			adjacent1->push_back(vertexID2);
		if (std::find(adjacent1->begin(), adjacent1->end(), vertexID3) == adjacent1->end())
			adjacent1->push_back(vertexID3);

		if (std::find(adjacent2->begin(), adjacent2->end(), vertexID1) == adjacent2->end())
			adjacent2->push_back(vertexID1);
		if (std::find(adjacent2->begin(), adjacent2->end(), vertexID3) == adjacent2->end())
			adjacent2->push_back(vertexID3);

		if (std::find(adjacent3->begin(), adjacent3->end(), vertexID1) == adjacent3->end())
			adjacent3->push_back(vertexID1);
		if (std::find(adjacent3->begin(), adjacent3->end(), vertexID2) == adjacent3->end())
			adjacent3->push_back(vertexID2);
	}

	int currentAdjIndex = 0;
	indexedMesh->minAdjNum = (int)adjacents[0].size();
	indexedMesh->maxAdjNum = 0;

	for (unsigned int i = 0; i < indexedMesh->nels; i++)
	{

		unsigned int currentAdjSize = (unsigned int)adjacents[i].size();

		unsigned int* adjIndexPtr = new unsigned int[1];
		*adjIndexPtr = ((unsigned int)currentAdjIndex) << 6;
		*adjIndexPtr += (currentAdjSize << 26) >> 26;
		float *f = (float*)adjIndexPtr;
		indexedMesh->Vertices[i].w = *f;

		//printf("indexOfAdjs  ->  %d   vs   %d\n", (*adjIndexPtr)>>6, currentAdjIndex);
		//printf("numOfAdjs  ->  %d   vs   %d\n", ((*adjIndexPtr)<<26)>>26, currentAdjSize);

		// Min & max adjacent number 
		if (currentAdjSize > indexedMesh->maxAdjNum) indexedMesh->maxAdjNum = currentAdjSize;
		else if (currentAdjSize < indexedMesh->minAdjNum) indexedMesh->minAdjNum = currentAdjSize;

		currentAdjIndex += currentAdjSize;
	}
	indexedMesh->nadjs = currentAdjIndex;

	// Now, currentAdjIndex is the tolal adjacents numbers.
	indexedMesh->adjArray = new unsigned int[currentAdjIndex];
	int adjIndex = 0;
	for (unsigned int i = 0; i < indexedMesh->nels; i++)
	{
		for (unsigned int vertexIndex : adjacents[i])
			indexedMesh->adjArray[adjIndex++] = vertexIndex;
	}










	indexedMesh->InitBuffers();
	return indexedMesh;
}

void IndexedMesh::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindVertexArray(VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

GLuint IndexedMesh::storeDataInAttributeListShared(int attributeNumber, int coordinateSize, GLvoid * data, int size)
{
	GLuint vboID; 
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, vboID);
	glBufferData(GL_ARRAY_BUFFER_ARB, size * coordinateSize * sizeof(GLfloat), data, GL_STATIC_DRAW_ARB);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	return vboID;
}


void IndexedMesh::Clear()
{
	//TO-DO
}