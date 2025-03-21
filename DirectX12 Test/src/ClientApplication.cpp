#include "ClientApplication.h"
#include <windowsx.h>

#include "DX12Engine/Rendering/RenderContext.h"
#include "DX12Engine/Rendering/Renderer.h"
#include "DX12Engine/Resources/Shader.h"
#include "DX12Engine/IO/ModelLoader.h"
#include "DX12Engine/Resources/Mesh.h"
#include "DX12Engine/Rendering/RenderObject.h"
#include "DX12Engine/IO/TextureLoader.h"
#include "DX12Engine/Resources/Texture.h"
#include "DX12Engine/Rendering/GPUUploader.h"
#include "DX12Engine/Resources/Materials/BasicMaterial.h"
#include "DX12Engine/Resources/Materials/PBRMaterial.h"
#include "DX12Engine/Resources/Materials/SkyboxMaterial.h"
#include "DX12Engine/Rendering/PipelineStateBuilder.h"
#include "DX12Engine/Rendering/RootSignatureBuilder.h"
#include "DX12Engine/Buffers/LightBuffer.h"
#include "DX12Engine/Resources/Skybox.h"

ClientApplication::ClientApplication()
{
	std::shared_ptr<DX12Engine::RenderContext> context = std::make_shared<DX12Engine::RenderContext>(this, 1600, 900);

	DX12Engine::Renderer renderer(context);

	DX12Engine::ModelLoader modelLoader;
	DX12Engine::Mesh mesh = modelLoader.LoadObj("E:\\Projects\\source\\repos\\DirectX12 Test\\cube2.obj");
	DX12Engine::Mesh mesh2 = modelLoader.LoadObj("E:\\Projects\\source\\repos\\DirectX12 Test\\cylinder.obj");

	DX12Engine::TextureLoader textureLoader;
	std::shared_ptr<DX12Engine::Texture> textureMC = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\minecraft_block_uv.png");
	std::shared_ptr<DX12Engine::Texture> textureWall = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\TCom_Wall_Stone3_2x2_512_albedo.tiff");
	std::shared_ptr<DX12Engine::Texture> skyboxDDS = textureLoader.LoadDDS(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\skybox\\kloofendal_48d_partly_cloudy_puresky_4k.dds");

	DX12Engine::GPUUploader uploader = context->GetUploader();
	std::vector<DX12Engine::Texture*> textures = { textureMC.get(), textureWall.get(), skyboxDDS.get() };
	uploader.UploadTextureBatch(textures);

	std::shared_ptr<DX12Engine::Texture> stoneAlbedo = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\dark-worn-stone-ue\\dark-worn-stonework_albedo.png");
	std::shared_ptr<DX12Engine::Texture> stoneNormal = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\dark-worn-stone-ue\\dark-worn-stonework_normal-dx.png");
	std::shared_ptr<DX12Engine::Texture> stoneMetallic = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\dark-worn-stone-ue\\dark-worn-stonework_metallic.png");
	std::shared_ptr<DX12Engine::Texture> stoneRoughness = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\dark-worn-stone-ue\\dark-worn-stonework_roughness.png");
	std::shared_ptr<DX12Engine::Texture> stoneAO = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\dark-worn-stone-ue\\dark-worn-stonework_ao.png");
	textures = { stoneAlbedo.get(), stoneNormal.get(), stoneMetallic.get(), stoneRoughness.get(), stoneAO.get() };
	uploader.UploadTextureBatch(textures);

	std::shared_ptr<DX12Engine::Texture> goldAlbedo = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\hammered-gold-ue\\hammered-gold_albedo.png");
	std::shared_ptr<DX12Engine::Texture> goldNormal = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\hammered-gold-ue\\hammered-gold_normal-dx.png");
	std::shared_ptr<DX12Engine::Texture> goldMetallic = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\hammered-gold-ue\\hammered-gold_metallic.png");
	std::shared_ptr<DX12Engine::Texture> goldRoughness = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\hammered-gold-ue\\hammered-gold_roughness.png");
	std::shared_ptr<DX12Engine::Texture> goldAO = textureLoader.LoadWIC(L"E:\\Projects\\source\\repos\\DirectX12 Test\\DirectX12 Test\\res\\hammered-gold-ue\\hammered-gold_ao.png");
	textures = { goldAlbedo.get(), goldNormal.get(), goldMetallic.get(), goldRoughness.get(), goldAO.get() };
	uploader.UploadTextureBatch(textures);

	DX12Engine::Skybox skybox(skyboxDDS);

	std::shared_ptr<DX12Engine::BasicMaterial> basicMaterial = std::make_shared<DX12Engine::BasicMaterial>();
	basicMaterial->SetTexture(goldAlbedo);

	std::shared_ptr<DX12Engine::PBRMaterial> pbrStone = std::make_shared<DX12Engine::PBRMaterial>();
	pbrStone->SetAlbedoMap(stoneAlbedo);
	pbrStone->SetNormalMap(stoneNormal);
	pbrStone->SetMetallicMap(stoneMetallic);
	pbrStone->SetRoughnessMap(stoneRoughness);
	pbrStone->SetAOMap(stoneAO);

	std::shared_ptr<DX12Engine::PBRMaterial> pbrGold = std::make_shared<DX12Engine::PBRMaterial>();
	pbrGold->SetAlbedoMap(goldAlbedo);
	pbrGold->SetNormalMap(goldNormal);
	pbrGold->SetMetallicMap(goldMetallic);
	pbrGold->SetRoughnessMap(goldRoughness);
	pbrGold->SetAOMap(goldAO);

	DX12Engine::RenderObject object1(mesh);
	DX12Engine::RenderObject object2(mesh2);
	object1.SetMaterial(pbrStone);
	object2.SetMaterial(pbrGold);
	object1.SetModelMatrix(DirectX::XMMatrixTranslation(-1.5f, 0.0f, 0.0f));
	object2.SetModelMatrix(DirectX::XMMatrixTranslation(1.5f, 0.0f, 0.0f));
	float count = 0.0f;

	DX12Engine::LightBuffer lightBuffer;
	DX12Engine::Light pointLight;
	pointLight.Type = (int)DX12Engine::LightType::Point;
	pointLight.Position = { 0.0f, 2.0f, 0.0f };
	pointLight.Intensity = 10.0f;
	pointLight.Range = 10.0f;
	pointLight.Color = { 1.0f, 1.0f, 1.0f };
	lightBuffer.AddLight(pointLight);
	DX12Engine::Light spotLight1;
	spotLight1.Type = (int)DX12Engine::LightType::Spot;
	spotLight1.Position = { -2.0f, 2.5f, 2.0f };
	spotLight1.Intensity = 10.0f;
	spotLight1.Color = { 0.0f, 0.0f, 1.0f };
	spotLight1.SpotAngle = cos(DirectX::XMConvertToRadians(-60.0f));
	//lightBuffer.AddLight(spotLight1);
	DX12Engine::Light spotLight2;
	spotLight2.Type = (int)DX12Engine::LightType::Spot;
	spotLight2.Position = { 2.0f, 2.5f, -2.0f };
	spotLight2.Intensity = 10.0f;
	spotLight2.Color = { 1.0f, 0.0f, 0.0f };
	spotLight2.SpotAngle = cos(DirectX::XMConvertToRadians(60.0f));
	//lightBuffer.AddLight(spotLight2);
	renderer.SetLightBuffer(&lightBuffer);

	m_Camera = std::make_unique<DX12Engine::Camera>(1600.0f / 900.0f, 0.001f, 100.0f);
	renderer.SetCamera(m_Camera.get());

	while (renderer.PollWindow())
	{
		m_Camera->ProcessKeyboardInput(0.01f);

		renderer.InitFrame(renderer.GetDefaultViewport(), renderer.GetDefaultScissorRect());
		//lightBuffer.GetLight(1)->SpotAngle = cos(DirectX::XMConvertToRadians(60.0f + count * 10));
		//lightBuffer.GetLight(2)->SpotAngle = sin(DirectX::XMConvertToRadians(60.0f + count * 10));
		lightBuffer.GetLight(0)->Position = { 2.0f * DirectX::XMScalarSin(count), 2.0f, 2.0f * DirectX::XMScalarCos(count) };
		lightBuffer.Update();
		//cube1.SetModelMatrix(DirectX::XMMatrixTranslation(-2.0f * DirectX::XMScalarSin(count), 0.0f, 0.0f));
		//cube2.SetModelMatrix(DirectX::XMMatrixTranslation(2.0f * DirectX::XMScalarSin(count), -0.5f, -0.5f));
		renderer.Render(&skybox);
		renderer.Render(&object1);
		renderer.Render(&object2);
		renderer.PresentFrame();
		count += 0.01f;
	}
}

ClientApplication::~ClientApplication()
{
}

void ClientApplication::HandleMouseMovement(HWND hwnd, LPARAM lParam)
{
	int mouseX = GET_X_LPARAM(lParam);
	int mouseY = GET_Y_LPARAM(lParam);

	if (m_FirstMouse) 
	{
		m_LastMouseX = (float)mouseX;
		m_LastMouseY = (float)mouseY;
		m_FirstMouse = false;
	}

	float deltaX = (float)(mouseX - m_LastMouseX);
	float deltaY = (float)(mouseY - m_LastMouseY);

	m_LastMouseX = (float)mouseX;
	m_LastMouseY = (float)mouseY;

	m_Camera->ProcessMouseInput(deltaX, deltaY);
}
