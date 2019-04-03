#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void DrawSky();

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();
	void GenerateLights();
	void GenerateMaterials();
	void InitStates();
	void InitVectors();

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* skyVS = nullptr;
	SimplePixelShader* skyPS = nullptr;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// Enetities
	std::vector<Entity*> entities;
	Entity * skyBox = nullptr;
	Entity * obj1 = nullptr;
	Entity * obj2 = nullptr;
	Entity * obj3 = nullptr;
	Entity * obj4 = nullptr;
	Entity * obj5 = nullptr;

	// Camera
	Camera * cam = nullptr;

	//Directional Light
	DirectionalLight dLight;
	DirectionalLight dLight2;

	// Point Lights
	PointLight pLight;
	PointLight pLight2;

	// Materials
	std::vector<Material*> materials;
	Material* skyBoxMaterial = nullptr;
	Material* testMat1 = nullptr;
	Material* testMat2 = nullptr;
	Material* testMat3 = nullptr;
	Material* testMat4 = nullptr;
	Material* testMat5 = nullptr;

	// Textures
	std::vector<Texture*> textures;
	Texture* skyBoxTexture = nullptr;
	Texture* testText1 = nullptr;
	Texture* testText2 = nullptr;
	Texture* testText3 = nullptr;
	Texture* testText4 = nullptr;
	Texture* testText5 = nullptr;
	

	// Normals
	std::vector<Texture*> normalMaps;
	Texture* testNorm1 = nullptr;
	Texture* testNorm2 = nullptr;
	Texture* testNorm3 = nullptr;
	Texture* testNorm4 = nullptr;
	Texture* testNorm5 = nullptr;

	// Loading in textures
	std::vector<ID3D11ShaderResourceView*> textureResources;
	ID3D11ShaderResourceView* srTest1 = nullptr;
	ID3D11ShaderResourceView* srTest2 = nullptr;
	ID3D11ShaderResourceView* srTest3 = nullptr;
	ID3D11ShaderResourceView* srTest4 = nullptr;
	ID3D11ShaderResourceView* srTest5 = nullptr;

	// Loading in Normals
	std::vector<ID3D11ShaderResourceView*> normalResources;
	ID3D11ShaderResourceView* srTestNormal1 = nullptr;
	ID3D11ShaderResourceView* srTestNormal2 = nullptr;
	ID3D11ShaderResourceView* srTestNormal3 = nullptr;
	ID3D11ShaderResourceView* srTestNormal4 = nullptr;
	ID3D11ShaderResourceView* srTestNormal5 = nullptr;

	// Render states
	ID3D11RasterizerState* rasterState = nullptr;
	ID3D11BlendState* blendState = nullptr;
	ID3D11DepthStencilState* depthState = nullptr;

	// SkyBox Resources
	ID3D11ShaderResourceView* skyResourceView = nullptr;
	ID3D11RasterizerState* skyRasterState = nullptr;
	ID3D11DepthStencilState* skyDepthState = nullptr;
};

