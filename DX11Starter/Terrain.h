#pragma once
#include <vector>
#include "Vertex.h"
#include "DXCore.h"
class Terrain
{
	int resolution, bitDepth, numVerts, numIndicies;
	ID3D11Device* device;
	float* heightArray;
	int* intArray;
	TerrainVertex* vertexArray;
	ID3D11Buffer* vertexBuffer, *indexBuffer;

public:
	Terrain(const char*, int, ID3D11Device*);
	~Terrain();
	void GenerateGrid();
	void CreateBuffers(Vertex* , int, unsigned int *, int, ID3D11Device *);
};