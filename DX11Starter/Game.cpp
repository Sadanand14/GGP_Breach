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
		delete entities[i]->meshObject;
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
	testText1 = new Texture();
	testText2 = new Texture();
	testText3 = new Texture();
	testText4 = new Texture();

	textures.push_back(skyBoxTexture);
	textures.push_back(testText1);
	textures.push_back(testText2);
	textures.push_back(testText3);
	textures.push_back(testText4);
	///

	/// Normal Maps
	testNorm1 = new Texture();
	testNorm2 = new Texture();
	testNorm3 = new Texture();
	testNorm4 = new Texture();

	normalMaps.push_back(testNorm1);
	normalMaps.push_back(testNorm2);
	normalMaps.push_back(testNorm3);
	normalMaps.push_back(testNorm4);
	///

	/// Materials
	skyBoxMaterial = new Material();
	testMat1 = new Material();
	testMat2 = new Material();
	testMat3 = new Material();
	testMat4 = new Material();

	materials.push_back(skyBoxMaterial);
	materials.push_back(testMat1);
	materials.push_back(testMat2);
	materials.push_back(testMat3);
	materials.push_back(testMat4);
	///

	textureResources.push_back(srTest1);
	textureResources.push_back(srTest2);
	textureResources.push_back(srTest3);
	textureResources.push_back(srTest4);

	normalResources.push_back(srTestNormal1);
	normalResources.push_back(srTestNormal2);
	normalResources.push_back(srTestNormal3);
	normalResources.push_back(srTestNormal4);
}

void Game::CreateBasicGeometry()
{
	cam = new Camera();
	cam->UpdateProjectionMatrix((float)width / height);

	// Making entities with materials and putting them in the vector
	// skyBox = new Entity("Assets/Models/cube.obj", device, skyBoxMaterial);
	// obj1 = new Entity("Assets/Models/sphere.obj", device, testMat1);
	// obj2 = new Entity("Assets/Models/sphere.obj", device, testMat2);
	// obj3 = new Entity("Assets/Models/sphere.obj", device, testMat3);
	// obj4 = new Entity("Assets/Models/sphere.obj", device, testMat4);

	scene = new Scene();

	skyBox = scene->SpawnEntity("Assets/Models/cube.obj", device, skyBoxMaterial);
	obj1 = scene->SpawnEntity("Assets/Models/sphere.obj", device, testMat1);
	obj2 = scene->SpawnEntity("Assets/Models/sphere.obj", device, testMat2);
	obj3 = scene->SpawnEntity("Assets/Models/sphere.obj", device, testMat3);
	obj4 = scene->SpawnEntity("Assets/Models/cube.obj", device, testMat4);

	entities.push_back(skyBox);
	entities.push_back(obj1);
	entities.push_back(obj2);
	entities.push_back(obj3);
	entities.push_back(obj4);
	///
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
	testText1->CreateTexure(device, context, L"Assets/Textures/testTextures/Colored.jpg", &srTest1);
	testText2->CreateTexure(device, context, L"Assets/Textures/testTextures/Rock.jpg", &srTest2);
	testText3->CreateTexure(device, context, L"Assets/Textures/testTextures/Marble.jpg", &srTest3);
	testText4->CreateTexure(device, context, L"Assets/Textures/testTextures/Wood.jpg", &srTest4);
	///

	/// Normals
	testNorm1->CreateTexure(device, context, L"Assets/Textures/testTextures/Colored_Normal.jpg", &srTestNormal1);
	testNorm2->CreateTexure(device, context, L"Assets/Textures/testTextures/Rock_Normal.jpg", &srTestNormal2);
	testNorm3->CreateTexure(device, context, L"Assets/Textures/testTextures/Marble_Normal.jpg", &srTestNormal3);
	testNorm4->CreateTexure(device, context, L"Assets/Textures/testTextures/Wood_Normal.jpg", &srTestNormal4);
	///

	// passing pixel and vertex to materials
	testMat1->CreateNormalMaterial(vertexShader, pixelShader, testText1->GetShaderResourceView(), testNorm1->GetShaderResourceView(), testText1->GetSamplerState());
	testMat2->CreateNormalMaterial(vertexShader, pixelShader, testText2->GetShaderResourceView(), testNorm2->GetShaderResourceView(), testText2->GetSamplerState());
	testMat3->CreateNormalMaterial(vertexShader, pixelShader, testText3->GetShaderResourceView(), testNorm3->GetShaderResourceView(), testText3->GetSamplerState());
	testMat4->CreateNormalMaterial(vertexShader, pixelShader, testText4->GetShaderResourceView(), testNorm4->GetShaderResourceView(), testText4->GetSamplerState());
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
	entities[0]->SetPositionF(0, 0, 0);
	entities[0]->SetScaleF(1, 1, 1);
	entities[0]->SetRotationF(0, 0, 0);
	///

	// entities[0]->SetWorldMatrix(entities[0]->CalculateWorldMatrix(entities[0]));

	/// Moving Entity 1
	entities[1]->SetPositionF(0, 0, 5);
	entities[1]->SetScaleF(3,3,3);
	entities[1]->SetRotationF(0,0,0);
	///

	// entities[1]->SetWorldMatrix(entities[1]->CalculateWorldMatrix(entities[1]));

	/// Moving Entity 2
	entities[2]->SetPositionF(-4 , 0, 5);
	entities[2]->SetScaleF(3, 3, 3);
	entities[2]->SetRotationF(0, 0, 0);
	///

	// entities[2]->SetWorldMatrix(entities[2]->CalculateWorldMatrix(entities[2]));

	/// Moving Entity 3
	entities[3]->SetPositionF(-8, 0, 5);
	entities[3]->SetScaleF(3, 3, 3);
	entities[3]->SetRotationF(0, 0, 0);

	// entities[3]->SetWorldMatrix(entities[3]->CalculateWorldMatrix(entities[3]));
	///

	/// Moving Entity 4
	entities[4]->SetPositionF(4, 0, 5);
	entities[4]->SetScaleF(3, 3, 3);
	entities[4]->SetRotationF(0, 0, 0);

	// entities[4]->SetWorldMatrix(entities[4]->CalculateWorldMatrix(entities[4]));
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