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

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// Enetities
	std::vector<Entity*> entities;
	Entity * obj1 = nullptr;
	Entity * obj2 = nullptr;
	Entity * obj3 = nullptr;
	Entity * obj4 = nullptr;

	// Camera
	Camera * cam = nullptr;

	//Directional Light
	DirectionalLight dLight;
	DirectionalLight dLight2;

	// Point Lights
	PointLight pLight;
	PointLight pLight2;

	// Materials
	Material* oblivionMaterial = nullptr;
	Material* keybladeMaterial = nullptr;
	Material* staffMaterial = nullptr;
	Material* shieldMaterial = nullptr;

	// Textures
	Texture* oblivionText;
	Texture* keybladeText;
	Texture* staffText;
	Texture* shieldText;

	// Loading in textures
	ID3D11ShaderResourceView* srOblivion = nullptr;
	ID3D11ShaderResourceView* srKeyblade = nullptr;
	ID3D11ShaderResourceView* srStaff = nullptr;
	ID3D11ShaderResourceView* srShield = nullptr;

	// Normals
	Texture* oblivionNorm;
	Texture* keybladeNorm;
	Texture* staffNorm;
	Texture* shieldNorm;

	// Loading in Normals
	ID3D11ShaderResourceView* srOblivionNormal = nullptr;
	ID3D11ShaderResourceView* srKeybladeNormal = nullptr;
	ID3D11ShaderResourceView* srStaffNormal = nullptr;
	ID3D11ShaderResourceView* srShieldNormal = nullptr;

	// Render states
	ID3D11RasterizerState* rasterState;
	ID3D11BlendState* blendState;
	ID3D11DepthStencilState* depthState;
};

