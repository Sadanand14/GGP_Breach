#include "Game.h"

#include "Scene.h"

// For the DirectX Math library
using namespace DirectX;

Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;
	skyPS = 0;
	skyVS = 0;
	shadowVS = 0;
	t = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	
}

Game::~Game()
{
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete skyVS;
	delete skyPS;
	delete shadowVS;

	// Deleting entities
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}

	// Deleting textures
	for (int i = 0; i < textures.size(); i++) {
		delete textures[i];
	}

	// Deleting textures
	for (int i = 0; i < normalMaps.size(); i++) {
		delete normalMaps[i];
	}

	// Deleting textures
	for (int i = 0; i < materials.size(); i++) {
		delete materials[i];
	}

	delete scene;

	// Deleting cam
	delete cam;

	blendState->Release();
	rasterState->Release();
	depthState->Release();
	skyDepthState->Release();
	skyRasterState->Release();

	// Deleting AI
	delete wayPtsAI;

	//Deleting Shadows
	shadowDSV->Release();
	shadowSRV->Release();
	shadowSamplerState->Release();
	shadowRasterizer->Release();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	LoadShaders();
	InitVectors();
	GenerateMaterials();
	InitStates();
	CreateBasicGeometry();
	GenerateLights();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	skyVS = new SimpleVertexShader(device, context);
	skyVS->LoadShaderFile(L"SkyVS.cso");

	skyPS = new SimplePixelShader(device, context);
	skyPS->LoadShaderFile(L"SkyPS.cso");

	shadowVS = new SimpleVertexShader(device, context);
	shadowVS->LoadShaderFile(L"ShadowVS.cso");
}

void Game::InitVectors()
{
	/// Textures
	skyBoxTexture = new Texture();
	battleship_Texture = new Texture();
	lightningTower_Texture = new Texture();
	airTower_Texture = new Texture();
	waterTower_Texture = new Texture();
	fireTower_Texture = new Texture();

	textures.push_back(skyBoxTexture);
	textures.push_back(battleship_Texture);
	textures.push_back(lightningTower_Texture);
	textures.push_back(airTower_Texture);
	textures.push_back(waterTower_Texture);
	textures.push_back(fireTower_Texture);
	///

	/// Normal Maps
	battleship_Normal = new Texture();
	lightningTower_Normal = new Texture();
	airTower_Normal = new Texture();
	waterTower_Normal = new Texture();
	fireTower_Normal = new Texture();

	normalMaps.push_back(battleship_Normal);
	normalMaps.push_back(lightningTower_Normal);
	normalMaps.push_back(airTower_Normal);
	normalMaps.push_back(waterTower_Normal);
	normalMaps.push_back(fireTower_Normal);

	///

	/// Materials
	skyBoxMaterial = new Material();
	battleship_Material = new Material();
	lightningTower_Material = new Material();
	airTower_Material = new Material();
	waterTower_Material = new Material();
	fireTower_Material = new Material();

	materials.push_back(skyBoxMaterial);
	materials.push_back(battleship_Material);
	materials.push_back(lightningTower_Material);
	materials.push_back(airTower_Material);
	materials.push_back(waterTower_Material);
	materials.push_back(fireTower_Material);
	///

	textureResources.push_back(battleshipSR);
	textureResources.push_back(lightningTowerSR);
	textureResources.push_back(airTowerSR);
	textureResources.push_back(waterTowerSR);
	textureResources.push_back(fireTowerSR);

	normalResources.push_back(battleship_NormalSR);
	normalResources.push_back(lightningTower_NormalSR);
	normalResources.push_back(airTower_NormalSR);
	normalResources.push_back(waterTower_NormalSR);
	normalResources.push_back(fireTower_NormalSR);
}

void Game::CreateBasicGeometry()
{
	cam = new Camera();
	cam->UpdateProjectionMatrix((float)width / height);
	scene = new Scene();

	meshes.reserve(10000);

	meshes.push_back(new Mesh("Assets/Models/cube.obj", device));
	meshes.push_back(new Mesh("Assets/Models/sphere.obj", device));
	meshes.push_back(new Mesh("Assets/Models/Battleship_TB.obj", device));
	meshes.push_back(new Mesh("Assets/Models/LightningTower.obj", device));
	meshes.push_back(new Mesh("Assets/Models/AirTower.obj", device));
	meshes.push_back(new Mesh("Assets/Models/WaterTower.obj", device));
	meshes.push_back(new Mesh("Assets/Models/FireTower.obj", device));

	skyBox         = scene->SpawnEntity(meshes[0], skyBoxMaterial);
	battleship     = scene->SpawnEntity(meshes[2], battleship_Material);
	lightningTower = scene->SpawnEntity(meshes[3], lightningTower_Material);
	airTower       = scene->SpawnEntity(meshes[4], airTower_Material);
	waterTower     = scene->SpawnEntity(meshes[5], waterTower_Material);
	fireTower      = scene->SpawnEntity(meshes[6], fireTower_Material);

	entities.push_back(skyBox);
	entities.push_back(battleship);
	entities.push_back(lightningTower);
	entities.push_back(airTower);
	entities.push_back(waterTower);
	entities.push_back(fireTower);
	///
	wayPtsAI = new AIBehaviors(entities[1]);
}

void Game::GenerateLights()
{
	/// Setting variables for both Directional lights
	dLight.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	dLight.DiffuseColor = XMFLOAT4(0.5f, 0.2f, 1, 1);
	dLight.Direction = XMFLOAT3(0, -1, 0);
	dLight.Shine = 25.0f;

	dLight2.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.05f, 1.0f);
	dLight2.DiffuseColor = XMFLOAT4(1, 0.2f, 0.3f, 1);
	dLight2.Direction = XMFLOAT3(0, 1, 0);
	dLight2.Shine = 52.0f;
	///

	/// Setting variables for both Point Light
	pLight.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	pLight.DiffuseColor = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.4f);
	pLight.Position = XMFLOAT3(0, 0, -1);
	pLight.Shine = 100.0f;
	pLight.Range = 20.0f;

	pLight2.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	pLight2.DiffuseColor = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.1f);
	pLight2.Position = XMFLOAT3(0, 0, 1);
	pLight2.Shine = 100.0f;
	pLight2.Range = 20.0f;
	///

	/// Sending all of the light data to the shaders for both lights
	pixelShader->SetData("DLight", &dLight, sizeof(DirectionalLight));

	pixelShader->SetData("DLight2", &dLight2, sizeof(DirectionalLight));

	pixelShader->SetData("PLight", &pLight, sizeof(PointLight));

	pixelShader->SetData("PLight2", &pLight2, sizeof(PointLight));
	///

	pixelShader->CopyAllBufferData();
}

void Game::GenerateMaterials()
{
	// Skybox Texture
	skyBoxTexture->CreateCubeMap(device, context, L"Assets/Textures/SunnyCubeMap.dds", &skyResourceView, &skyRasterState, &skyDepthState);
	skyBoxMaterial->CreateMaterial(skyVS, skyPS, skyBoxTexture->GetShaderResourceView(), skyBoxTexture->GetSamplerState());

	/// Textures
	battleship_Texture ->CreateTexure(device, context, L"Assets/Textures/BattleShip_Texture.png", &battleshipSR);
	lightningTower_Texture->CreateTexure(device, context, L"Assets/Textures/LightningTower_Texture.png", &lightningTowerSR);
	airTower_Texture->CreateTexure(device, context, L"Assets/Textures/AirTower_Texture.png", &airTowerSR);
	waterTower_Texture->CreateTexure(device, context, L"Assets/Textures/WaterTower_Texture.png", &waterTowerSR);
	fireTower_Texture->CreateTexure(device, context, L"Assets/Textures/FireTower_Texture.png", &fireTowerSR);
	///

	/// Normals
	battleship_Normal->CreateTexure(device, context, L"Assets/Textures/testTextures/Colored_Normal.jpg", &battleship_NormalSR);
	lightningTower_Normal->CreateTexure(device, context, L"Assets/Textures/testTextures/Rock_Normal.jpg", &lightningTower_NormalSR);
	airTower_Normal->CreateTexure(device, context, L"Assets/Textures/testTextures/Marble_Normal.jpg", &airTower_NormalSR);
	waterTower_Normal->CreateTexure(device, context, L"Assets/Textures/testTextures/Wood_Normal.jpg", &waterTower_NormalSR);
	fireTower_Normal->CreateTexure(device, context, L"Assets/Textures/testTextures/Wood_Normal.jpg", &fireTower_NormalSR);
	///

	// passing pixel and vertex to materials
	battleship_Material->CreateMaterial(vertexShader, pixelShader, battleship_Texture->GetShaderResourceView(), battleship_Texture->GetSamplerState());
	lightningTower_Material->CreateMaterial(vertexShader, pixelShader, lightningTower_Texture->GetShaderResourceView(), lightningTower_Texture->GetSamplerState());
	airTower_Material->CreateMaterial(vertexShader, pixelShader, airTower_Texture->GetShaderResourceView(), airTower_Texture->GetSamplerState());
	waterTower_Material->CreateMaterial(vertexShader, pixelShader, waterTower_Texture->GetShaderResourceView(), waterTower_Texture->GetSamplerState());
	fireTower_Material->CreateMaterial(vertexShader, pixelShader, fireTower_Texture->GetShaderResourceView(), fireTower_Texture->GetSamplerState());
}

void Game::InitStates()
{
	/// Raster State
	D3D11_RASTERIZER_DESC rd = {};
	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_SOLID;
	device->CreateRasterizerState(&rd, &rasterState);

	// Turn on the rasterizer state (note: this is usually done
	// inside Draw() as necessary for each object, but we're doing
	// it here cause it's a simple demo)
	context->RSSetState(rasterState);
	///

	/// Depth State
	D3D11_DEPTH_STENCIL_DESC ds = {};
	ds.DepthEnable = false;
	device->CreateDepthStencilState(&ds, &depthState);
	//context->OMSetDepthStencilState(depthState, 0);
	///

	/// Blend State
	D3D11_BLEND_DESC bd = {};
	bd.RenderTarget[0].BlendEnable = true;

	// Settings for blending RGB channels
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// Settings for blending alpha channel
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	// Setting for masking out individual color channels
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Create the state
	device->CreateBlendState(&bd, &blendState);

	// Set the state! (For last param, set all the bits!)
	context->OMSetBlendState(blendState, 0, 0xFFFFFFFF);

	//Shadowmap init
	nShadowMapSize = 2048;

	D3D11_TEXTURE2D_DESC shadowDesc = {};
	shadowDesc.Width = nShadowMapSize;
	shadowDesc.Height = nShadowMapSize;
	shadowDesc.ArraySize = 1;
	shadowDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowDesc.CPUAccessFlags = 0;
	shadowDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowDesc.MipLevels = 1;
	shadowDesc.MiscFlags = 0;
	shadowDesc.SampleDesc.Count = 1;
	shadowDesc.SampleDesc.Quality = 0;
	shadowDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* shadowTexture;
	device->CreateTexture2D(&shadowDesc, 0, &shadowTexture);

	// Create the depth/stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSDesc = {};
	shadowDSDesc.Format = DXGI_FORMAT_D32_FLOAT;
	shadowDSDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowDSDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(shadowTexture, &shadowDSDesc, &shadowDSV);

	// Create the SRV for the shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(shadowTexture, &srvDesc, &shadowSRV);

	// Release the texture reference since we don't need it
	shadowTexture->Release();

	// Create the special "comparison" sampler state for shadows
	D3D11_SAMPLER_DESC shadowSampDesc = {};
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; // Could be anisotropic
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	device->CreateSamplerState(&shadowSampDesc, &shadowSamplerState);

	// Create a rasterizer state
	D3D11_RASTERIZER_DESC shadowRastDesc = {};
	shadowRastDesc.FillMode = D3D11_FILL_SOLID;
	shadowRastDesc.CullMode = D3D11_CULL_BACK;
	shadowRastDesc.DepthClipEnable = true;
	shadowRastDesc.DepthBias = 1000; // Multiplied by (smallest possible value > 0 in depth buffer)
	shadowRastDesc.DepthBiasClamp = 0.0f;
	shadowRastDesc.SlopeScaledDepthBias = 1.0f;
	device->CreateRasterizerState(&shadowRastDesc, &shadowRasterizer);
}



void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	cam->UpdateProjectionMatrix((float)width / height);
}

void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// Camera
	cam->Update(deltaTime);

	XMFLOAT3 camPosHolder;
	XMStoreFloat3(&camPosHolder, cam->GetCameraPostion());
	pixelShader->SetFloat3("cameraPos", camPosHolder);

	float sinTime = (sin(totalTime * 10) + 2.0f) / 5.0f;

	// SkyBox
	entities[0]->SetPositionF(0, 0, 0);
	entities[0]->SetScaleF(1, 1, 1);
	entities[0]->SetRotationF(0, 0, 0);
	///

	/// Moving Entity 1
	entities[1]->SetPositionF(0, 0, 5);
	entities[1]->SetScaleF(3,3,3);
	entities[1]->SetRotationF(0,0,0);
	///

	/// Moving Entity 2
	entities[2]->SetPositionF(-4 , 0, 5);
	entities[2]->SetScaleF(3, 3, 3);
	entities[2]->SetRotationF(0, 0, 0);
	///

	/// Moving Entity 3
	entities[3]->SetPositionF(-8, 0, 5);
	entities[3]->SetScaleF(3, 3, 3);
	entities[3]->SetRotationF(0, 0, 0);
	///

	/// Moving Entity 4
	entities[4]->SetPositionF(4, 0, 5);
	entities[4]->SetScaleF(3, 3, 3);
	entities[4]->SetRotationF(0, 0, 0);
	///

	switch (wayPtsAI->lerpPeriods) {
		//case 0: wayPtsAI->WaypointsLerp(entities[2]->GetWorldPosition(), entities[4]->GetWorldPosition()); break;
		//case 1: wayPtsAI->WaypointsLerp(entities[4]->GetWorldPosition(), entities[3]->GetWorldPosition()); break;
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	RenderShadowMap();

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	vertexShader->SetMatrix4x4("view", cam->GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", cam->GetProjectionMatrix());

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("Sky", skyResourceView);
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (int i = 1; i < entities.size(); i++) {
		entities[i]->PrepareShader(cam->GetViewMatrix(), cam->GetProjectionMatrix());

		mat4 worldMat = glm::transpose(entities[i]->GetWorldMatrix());
		float* matarr = &(worldMat[0][0]);

		vertexShader->SetMatrix4x4("world", matarr);
		vertexShader->CopyAllBufferData();

		ID3D11Buffer * passVB = entities[i]->meshObject->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &passVB, &stride, &offset);
		context->IASetIndexBuffer(entities[i]->meshObject->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			entities[i]->meshObject->GetIndexCount(),
			0,
			0);
	}

	// Draw the sky AFTER all opaque geometry
	DrawSky();

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}

void Game::DrawSky()
{
	ID3D11Buffer* skyVB = entities[0]->meshObject->GetVertexBuffer();
	ID3D11Buffer* skyIB = entities[0]->meshObject->GetIndexBuffer();

	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
	context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

	// Set up shaders
	skyVS->SetMatrix4x4("view", cam->GetViewMatrix());
	skyVS->SetMatrix4x4("projection", cam->GetProjectionMatrix());
	skyVS->CopyAllBufferData();
	skyVS->SetShader();

	skyPS->SetShaderResourceView("Sky", skyBoxMaterial->GetShaderResourceView());
	skyPS->SetSamplerState("BasicSampler", skyBoxMaterial->GetSamplerState());
	skyPS->SetShader();


	// Set up sky-specific render states
	context->RSSetState(skyRasterState);
	context->OMSetDepthStencilState(skyDepthState, 0);

	// Draw
	context->DrawIndexed(entities[0]->meshObject->GetIndexCount(), 0, 0);

	// Reset your states
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}

void Game::RenderShadowMap()
{
	// Initial setup of targets and states
	context->OMSetRenderTargets(0, 0, shadowDSV);
	context->ClearDepthStencilView(shadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->RSSetState(shadowRasterizer);


	D3D11_VIEWPORT shadowViewport = {};
	shadowViewport.TopLeftX = 0;
	shadowViewport.TopLeftY = 0;
	shadowViewport.Width = (float)nShadowMapSize;
	shadowViewport.Height = (float)nShadowMapSize;
	shadowViewport.MinDepth = 0.0f;
	shadowViewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &shadowViewport);


	shadowVS->SetShader();
	shadowVS->SetMatrix4x4("view", dirLightView);
	shadowVS->SetMatrix4x4("projection", dirLightProjection);


	context->PSSetShader(0, 0, 0); // Unbinds the pixel shader


	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (UINT i = 0; i < entities.size(); i++)
	{

		Entity* currentEntity = entities[i];
		ID3D11Buffer* vb = currentEntity->meshObject->GetVertexBuffer();
		ID3D11Buffer* ib = currentEntity->meshObject->GetIndexBuffer();

		// printf("%d    %d", vb, ib);


		context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		mat4 worldMat = glm::transpose(entities[i]->GetWorldMatrix());
		float* matarr = &(worldMat[0][0]);

		shadowVS->SetMatrix4x4("world", matarr);
		shadowVS->CopyAllBufferData();


		context->DrawIndexed(currentEntity->meshObject->GetIndexCount(), 0, 0);
	}

	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
	context->RSSetState(0);
	shadowViewport.Width = (float)this->width;
	shadowViewport.Height = (float)this->height;
	context->RSSetViewports(1, &shadowViewport);

}


#pragma region Mouse Input

void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// passing mouse values for camera rotation
	if (buttonState & 0x0001) {
		cam->RotateCamera((x - prevMousePos.x)/160.0f, (y - prevMousePos.y)/160.0f);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion