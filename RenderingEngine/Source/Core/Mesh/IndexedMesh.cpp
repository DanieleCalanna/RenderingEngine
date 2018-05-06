#include "Core/Mesh/IndexedMesh.hpp"
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

using namespace std;

void splitfill(std::string const& s, const char delimiter, unsigned int output[])
{
	size_t start = 0;
	size_t end = s.find_first_of(delimiter);

	std::string str;
	int i = 0;

	output[1] = 0;
	output[2] = 0;

	while (end <= std::string::npos)
	{
		str = s.substr(start, end - start);

		output[i++] = (str != "") ? stoi(str) : 0;

		if (end == std::string::npos) break;

		start = end + 1;
		end = s.find_first_of(delimiter, start);
	}
}

IndexedMesh::IndexedMesh(std::string ObjFilePath)
{
	LoadObj(ObjFilePath);
	InitBuffers();
}

void IndexedMesh::InitBuffers()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexBuffer = StoreDataInAttributeListShared(0, 4, &Vertices[0], (int)Vertices.size());
	normalsBuffer = StoreDataInAttributeListShared(1, 4, &Normals[0], (int)Normals.size());
	StoreDataInAttributeListShared(2, 2, &UVs[0], (int)UVs.size());
	StoreDataInAttributeListShared(3, 3, &Tangents[0], (int)Tangents.size());
	StoreDataInAttributeListShared(4, 3, &Bitangents[0], (int)Bitangents.size());
	glBindVertexArray(0);
}

void IndexedMesh::LoadObj(std::string ObjFilePath)
{
	Clear();

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;

	std::vector< glm::vec4 > ObjVertices;
	std::vector< glm::vec2 > ObjUVs;
	std::vector< glm::vec4 > ObjNormals;

	ifstream ObjFile(ObjFilePath);
	if (!ObjFile.is_open())
	{
		std::cout << "Impossible to open the file !" << std::endl;
		return;
	}

	std::stringstream ObjStringStream;
	ObjStringStream << ObjFile.rdbuf();

	std::string Keyword, a, b, c;

	unsigned int Face1[3] { 0 }, Face2[3] { 0 }, Face3[3] { 0 };

	while (ObjStringStream >> Keyword)
	{
		if (Keyword == "v")
		{
			ObjStringStream >> a >> b >> c;
			ObjVertices.push_back(glm::vec4(stof(a), stof(b), stof(c), 1.0f));
		}
		else if (Keyword == "vn")
		{
			ObjStringStream >> a >> b >> c;
			ObjNormals.push_back(glm::vec4(stof(a), stof(b), stof(c), 0.0f));
		}
		else if (Keyword == "vt")
		{
			ObjStringStream >> a >> b;
			ObjUVs.push_back(glm::vec2(stof(a), stof(b)));
		}
		else if (Keyword == "f")
		{
			ObjStringStream >> a >> b >> c;

			splitfill(a, '/', Face1);
			splitfill(b, '/', Face2);
			splitfill(c, '/', Face3);

			Indices.push_back(Face1[0] - 1);
			Indices.push_back(Face2[0] - 1);
			Indices.push_back(Face3[0] - 1);

			vertexIndices.push_back(Face1[0]);
			vertexIndices.push_back(Face2[0]);
			vertexIndices.push_back(Face3[0]);
			uvIndices.push_back(Face1[1]);
			uvIndices.push_back(Face2[1]);
			uvIndices.push_back(Face3[1]);
			normalIndices.push_back(Face1[2]);
			normalIndices.push_back(Face2[2]);
			normalIndices.push_back(Face3[2]);
		}
	}
	/*
	string LineOfObjFile;
	while (getline(ObjFile, LineOfObjFile))
	{
		std::vector<std::string> WordsInLine = split(LineOfObjFile, ' ');
		if (WordsInLine.size() == 0) continue;

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
			std::vector<std::string> ParamsOfVertex0 = split(WordsInLine[1], '/');
			std::vector<std::string> ParamsOfVertex1 = split(WordsInLine[2], '/');
			std::vector<std::string> ParamsOfVertex2 = split(WordsInLine[3], '/');

			unsigned int FaceVerticesIndices[3], FaceUVsIndices[3], FaceNormalsIndices[3];

			FaceVerticesIndices[0] = stoi(ParamsOfVertex0[0]);
			FaceUVsIndices[0] = stoi(ParamsOfVertex0[1]);
			FaceNormalsIndices[0] = stoi(ParamsOfVertex0[2]);

			FaceVerticesIndices[1] = stoi(ParamsOfVertex1[0]);
			FaceUVsIndices[1] = stoi(ParamsOfVertex1[1]);
			FaceNormalsIndices[1] = stoi(ParamsOfVertex1[2]);

			FaceVerticesIndices[2] = stoi(ParamsOfVertex2[0]);
			FaceUVsIndices[2] = stoi(ParamsOfVertex2[1]);
			FaceNormalsIndices[2] = stoi(ParamsOfVertex2[2]);

			Indices.push_back(FaceVerticesIndices[0] - 1);
			Indices.push_back(FaceVerticesIndices[1] - 1);
			Indices.push_back(FaceVerticesIndices[2] - 1);

			vertexIndices.push_back(FaceVerticesIndices[0]);
			vertexIndices.push_back(FaceVerticesIndices[1]);
			vertexIndices.push_back(FaceVerticesIndices[2]);
			uvIndices.push_back(FaceUVsIndices[0]);
			uvIndices.push_back(FaceUVsIndices[1]);
			uvIndices.push_back(FaceUVsIndices[2]);
			normalIndices.push_back(FaceNormalsIndices[0]);
			normalIndices.push_back(FaceNormalsIndices[1]);
			normalIndices.push_back(FaceNormalsIndices[2]);
		}
	}
	*/
	Vertices.resize(ObjVertices.size());
	Normals.resize(ObjVertices.size());
	UVs.resize(ObjVertices.size());
	//Tangents.resize(ObjVertices.size());
	//Bitangents.resize(ObjVertices.size());

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int VertexIndex = vertexIndices[i] - 1;
		unsigned int NormalIndex = normalIndices[i] - 1;
		unsigned int UVIndex = uvIndices[i] - 1;

		Vertices[VertexIndex] = ObjVertices[VertexIndex];
		Normals[VertexIndex] = ObjNormals[NormalIndex];
		UVs[VertexIndex] = ObjUVs[UVIndex];
	}
	
	for (int i = 0; i < Indices.size(); i += 3)
	{
		// Shortcuts for vertices
		glm::vec3 v0 = Vertices[Indices[i + 0]];
		glm::vec3 v1 = Vertices[Indices[i + 1]];
		glm::vec3 v2 = Vertices[Indices[i + 2]];

		// Shortcuts for UVs
		glm::vec2 & uv0 = UVs[Indices[i + 0]];
		glm::vec2 & uv1 = UVs[Indices[i + 1]];
		glm::vec2 & uv2 = UVs[Indices[i + 2]];

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
		Tangents.push_back(Tangent);
		Tangents.push_back(Tangent);
		Tangents.push_back(Tangent);

		// Same thing for bitangents
		Bitangents.push_back(Bitangent);
		Bitangents.push_back(Bitangent);
		Bitangents.push_back(Bitangent);
	}
	

	// Init number of vertex
	nels = (unsigned int)Vertices.size();
	// Discover adjacents vertex for each vertex
	std::vector< unsigned int >* adjacents = new std::vector< unsigned int >[nels];

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
	minAdjNum = (unsigned int)adjacents[0].size();
	maxAdjNum = 0;

	for (unsigned int i = 0; i < nels; i++)
	{
		unsigned int currentAdjSize = (unsigned int)adjacents[i].size();

		unsigned int* adjIndexPtr = new unsigned int[1];
		*adjIndexPtr = ((unsigned int)currentAdjIndex) << 6;
		*adjIndexPtr += (currentAdjSize << 26) >> 26;
		float *f = (float*)adjIndexPtr;
		Vertices[i].w = *f;

		//printf("indexOfAdjs  ->  %d   vs   %d\n", (*adjIndexPtr)>>6, currentAdjIndex);
		//printf("numOfAdjs  ->  %d   vs   %d\n", ((*adjIndexPtr)<<26)>>26, currentAdjSize);

		// Min & max adjacent number 
		if (currentAdjSize > maxAdjNum) maxAdjNum = currentAdjSize;
		else if (currentAdjSize < minAdjNum) minAdjNum = currentAdjSize;

		currentAdjIndex += currentAdjSize;
	}
	nadjs = currentAdjIndex;

	// Now, currentAdjIndex is the tolal adjacents numbers.
	adjArray = new unsigned int[currentAdjIndex];
	int adjIndex = 0;
	for (unsigned int i = 0; i < nels; i++)
	{
		for (unsigned int vertexIndex : adjacents[i])
			adjArray[adjIndex++] = vertexIndex;
	}
}

void IndexedMesh::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glBindVertexArray(VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	
	glDrawElements(GL_TRIANGLES, (GLsizei)Indices.size(), GL_UNSIGNED_INT, (void*)0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glBindVertexArray(0);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

GLuint IndexedMesh::StoreDataInAttributeListShared(unsigned int AttributeIndex, int CoordinateSize, GLvoid * Data, int NumberOfElements)
{
	GLuint VertexBufferID;
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER_ARB, NumberOfElements * CoordinateSize * sizeof(GLfloat), Data, GL_STATIC_DRAW_ARB);
	glVertexAttribPointer(AttributeIndex, CoordinateSize, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	return VertexBufferID;
}


void IndexedMesh::Clear()
{
	Mesh::Clear();
	//TO-DO
}
