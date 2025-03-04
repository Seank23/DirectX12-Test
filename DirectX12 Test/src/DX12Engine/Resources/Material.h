#pragma once
#include "d3d12.h"
#include <wrl.h>
#include <DirectXMath.h>
#include "./Texture.h"
#include "../Rendering/PipelineStateBuilder.h" 
#include "../Rendering/RootSignatureBuilder.h"
#include "../Buffers/ConstantBuffer.h"

namespace DX12Engine
{
	struct MaterialData
	{
		DirectX::XMFLOAT4 BaseColor;
		int HasTexture = 0;
	};

	class Material
	{
	public:
		Material(DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
		~Material();

		void SetColor(DirectX::XMFLOAT4 color);

		void SetTexture(std::shared_ptr<Texture> texture);
		void BuildPipelineState();

		bool HasTexture() const { return m_Texture != nullptr; }

		D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle() { return m_Texture->GetDescriptor()->GetGPUHandle(); }
		D3D12_GPU_VIRTUAL_ADDRESS GetCBVAddress() { return m_ConstantBuffer->GetGPUAddress(); }
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() { return m_PipelineState; }
		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() { return m_RootSignature; }

		void ConfigureFromDefault(Shader* vertexShader, Shader* pixelShader);

		PipelineStateBuilder PipelineStateBuilder;
		RootSignatureBuilder RootSignatureBuilder;

	private:
		void UpdateConstantBufferData();

		MaterialData m_MaterialData;
		std::shared_ptr<Texture> m_Texture;
		std::unique_ptr<ConstantBuffer> m_ConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PipelineState;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;
	};
}
