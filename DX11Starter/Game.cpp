#include "Game.h"

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

	// Deleting entities
	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
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

	// Deleting cam
	delete cam;

	blendState->Release();
	rasterState->Release();
	depthState->Release();
	skyDepthState->Release();
	skyRasterState->Release();

	// Deleting AI
	delete wayPtsAI;
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

	// Making entities with materials and putting them in the vector
	skyBox = new Entity("Assets/Models/cube.obj", device, skyBoxMaterial);
	battleship = new Entity("Assets/Models/Battleship_TB.obj", device, battleship_Material);
	lightningTower = new Entity("Assets/Models/LightningTower.obj", device, lightningTower_Material);
	airTower = new Entity("Assets/Models/AirTower.obj", device, airTower_Material);
	waterTower = new Entity("Assets/Models/WaterTower.obj", device, waterTower_Material);
	fireTower = new Entity("Assets/Models/FireTower.obj", device, fireTower_Material);

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
	entities[0]->SetPosition(0, 0, 0);
	entities[0]->SetScale(1, 1, 1);
	entities[0]->SetRotation(0, 0, 0);
	
	///

	entities[0]->SetWorldMatrix(entities[0]->CalculateWorldMatrix(entities[0]));

	/// Moving Entity 1
	// entities[1]->SetPosition(0, 0, 5);

	// entities[1]->SetPosition(-1.5f + 3 * totalTime, 0, 5);
	entities[1]->SetScale(3,3,3);
	entities[1]->SetRotation(0,0,0);

	wayPtsAI->WaypointsLerp({ XMFLOAT3(0,0,1), XMFLOAT3(0,0,1) , XMFLOAT3(0,0,1) , XMFLOAT3(0,0,1) , XMFLOAT3(0,0,1) }, totalTime);
	///

	entities[1]->SetWorldMatrix(entities[1]->CalculateWorldMatrix(entities[1]));

	/// Moving Entity 2
	entities[2]->SetPosition(-4 , 0, 5);
	entities[2]->SetScale(3, 3, 3);
	entities[2]->SetRotation(0, 0, 0);
	///

	entities[2]->SetWorldMatrix(entities[2]->CalculateWorldMatrix(entities[2]));

	/// Moving Entity 3
	entities[3]->SetPosition(-8, 0, 5);
	entities[3]->SetScale(3, 3, 3);
	entities[3]->SetRotation(0, 0, 0);

	entities[3]->SetWorldMatrix(entities[3]->CalculateWorldMatrix(entities[3]));
	///

	/// Moving Entity 4
	entities[4]->SetPosition(4, 0, 5);
	entities[4]->SetScale(3, 3, 3);
	entities[4]->SetRotation(0, 0, 0);

	entities[4]->SetWorldMatrix(entities[4]->CalculateWorldMatrix(entities[4]));
	///
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
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

		vertexShader->SetMatrix4x4("world", entities[i]->GetWorldMatrix());
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