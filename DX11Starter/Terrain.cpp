#include "Terrain.h"
#include "Vertex.h"
#include <fstream>

Terrain::Terrain(const char* fileLocation, int resolution, ID3D11Device* dev) 
{
	device = dev;
	
	std::vector<unsigned char>	heights(resolution * resolution);
	heightArray = new float[resolution*resolution];
	vertexArray = new TerrainVertex[resolution*resolution];
	intArray = new int[resolution*resolution];

	std::ifstream file;
	file.open(fileLocation, std::ios_base::binary);

	if (file)
	{
		file.read((char*)&heights[0], (std::streamsize)file.tellg());
		file.close();
	}

	//std::vector<float> finalHeights(resolution * resolution);
	
	for (int i = 0; i < resolution * resolution; i++)
	{
		heightArray[i] = (heights[i] / 255.0f)* 0.5;
	}
}

Terrain::~Terrain() 
{
	for (int i = 0; i < resolution*resolution; i++) 
	{
		delete &vertexArray[i];
	}
	delete heightArray;
	delete vertexArray;
	delete intArray;
	delete vertexBuffer;
}

void Terrain::GenerateGrid() 
{
	using namespace DirectX;
	for (int i = 0; i < resolution; i++) 
	{
		for (int j = 0; i < resolution; j++) 
		{
			TerrainVertex V = TerrainVertex();
			V.Position = XMFLOAT3(i,heightArray[i+j],j);
			V.UV = XMFLOAT2(0, 0);
			V.Normal = XMFLOAT3(0,1,0);
			vertexArray[i + j] = V;
		}
	}
}

void Terrain::CreateBuffers(Vertex * verts, int numVerts, unsigned int * inds, int numInd, ID3D11Device * createBuff)
{
	numIndicies = numInd;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numInd;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = verts;

	// Actually create the buffer with the initial data
	createBuff->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * numInd;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = inds;

	// Actually create the buffer with the initial data
	createBuff->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);
}