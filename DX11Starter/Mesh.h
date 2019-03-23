#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"
#include <vector>
#include <fstream>

class Mesh
{
public:
	// Constructor and Deconstructor
	Mesh(Vertex * verts, int numVerts, unsigned int* inds, int numInd, ID3D11Device * createBuff);
	Mesh(char* filename, ID3D11Device * createBuff);
	~Mesh();

	// Getter methods
	ID3D11Buffer * GetVertexBuffer();
	ID3D11Buffer * GetIndexBuffer();
	int GetIndexCount();

private:
	void CreateBuffers(Vertex * verts, int numVerts, unsigned int* inds, int numInd, ID3D11Device * createBuff);
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);

	// Buffer pointers
	ID3D11Buffer * vertexBuff;
	ID3D11Buffer * indexBuff;
	int numIndicies;
};

