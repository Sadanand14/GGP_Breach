#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"
#include "AIBehaviors.h"
#include <vector>

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
	void RenderShadowMap();

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void CreateBasicGeometry();
	void GenerateLights();
	void GenerateMaterials();
	void InitStates();
	void InitVectors();
	void LoadHeightTexture();
	void GenerateTerrainVertices(std::vector<float> heightList);

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;
	SimpleVertexShader* skyVS = nullptr;
	SimplePixelShader* skyPS = nullptr;
	SimpleVertexShader* terrainVS = nullptr;
	SimplePixelShader* terrainPS = nullptr;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// The scene
	Scene* scene;

	// Enetities
	std::vector<Entity*> entities;
	Entity * skyBox = nullptr;

	Entity * battleship = nullptr;
	Entity * lightningTower = nullptr;
	Entity * airTower = nullptr;
	Entity * waterTower = nullptr;
	Entity * fireTower = nullptr;


	// Camera
	Camera * cam = nullptr;

	//Directional Light
	DirectionalLight dLight;
	DirectionalLight dLight2;

	// Point Lights
	PointLight pLight;
	PointLight pLight2;

	// Meshes
	std::vector<Mesh*> meshes;

	// Materials
	std::vector<Material*> materials;
	Material* skyBoxMaterial = nullptr;
	Material* battleship_Material = nullptr;
	Material* lightningTower_Material = nullptr;
	Material* airTower_Material = nullptr;
	Material* waterTower_Material = nullptr;
	Material* fireTower_Material = nullptr;

	// Textures
	std::vector<Texture*> textures;
	Texture* skyBoxTexture = nullptr;
	Texture* battleship_Texture = nullptr;
	Texture* lightningTower_Texture = nullptr;
	Texture* airTower_Texture = nullptr;
	Texture* waterTower_Texture = nullptr;
	Texture* fireTower_Texture = nullptr;

	// Normals
	std::vector<Texture*> normalMaps;
	Texture* battleship_Normal = nullptr;
	Texture* lightningTower_Normal = nullptr;
	Texture* airTower_Normal = nullptr;
	Texture* waterTower_Normal = nullptr;
	Texture* fireTower_Normal = nullptr;

	// Loading in textures
	std::vector<ID3D11ShaderResourceView*> textureResources;
	ID3D11ShaderResourceView* battleshipSR = nullptr;
	ID3D11ShaderResourceView* lightningTowerSR = nullptr;
	ID3D11ShaderResourceView* airTowerSR = nullptr;
	ID3D11ShaderResourceView* waterTowerSR = nullptr;
	ID3D11ShaderResourceView* fireTowerSR = nullptr;

	// Loading in Normals
	std::vector<ID3D11ShaderResourceView*> normalResources;
	ID3D11ShaderResourceView* battleship_NormalSR = nullptr;
	ID3D11ShaderResourceView* lightningTower_NormalSR = nullptr;
	ID3D11ShaderResourceView* airTower_NormalSR = nullptr;
	ID3D11ShaderResourceView* waterTower_NormalSR = nullptr;
	ID3D11ShaderResourceView* fireTower_NormalSR = nullptr;

	// Render states
	ID3D11RasterizerState* rasterState = nullptr;
	ID3D11BlendState* blendState = nullptr;
	ID3D11DepthStencilState* depthState = nullptr;

	// SkyBox Resources
	ID3D11ShaderResourceView* skyResourceView = nullptr;
	ID3D11RasterizerState* skyRasterState = nullptr;
	ID3D11DepthStencilState* skyDepthState = nullptr;

	// AI
	AIBehaviors* wayPtsAI = nullptr;
	float t;

	//Shadowmap Resources
	int nShadowMapSize;
	ID3D11DepthStencilView* shadowDSV;
	ID3D11ShaderResourceView* shadowSRV;
	ID3D11SamplerState* shadowSamplerState;
	ID3D11RasterizerState* shadowRasterizer;
	SimpleVertexShader* shadowVS;
	DirectX::XMFLOAT4X4 dirLightProjection;
	DirectX::XMFLOAT4X4 dirLightView;
};

