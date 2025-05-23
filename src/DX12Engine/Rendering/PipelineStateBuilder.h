#pragma once
#include <d3dx12.h>
#include "../Resources/Shader.h"

namespace DX12Engine
{
    class PipelineStateBuilder 
    {
    public:
        PipelineStateBuilder() 
        {
            ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
            inputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
            inputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
            inputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
            inputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
        }

        PipelineStateBuilder& ConfigureFromDefault(Shader* vertexShader = nullptr, Shader* pixelShader = nullptr)
        {
            if (vertexShader != nullptr && pixelShader != nullptr)
            {
                return AddInputLayout()
                    .SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT))
                    .SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT))
                    .SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT))
                    .SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
                    .SetRenderTargets({ DXGI_FORMAT_R8G8B8A8_UNORM })
					.SetDepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT)
                    .SetSampleDesc(UINT_MAX, 1, 0)
                    .SetVertexShader(vertexShader)
                    .SetPixelShader(pixelShader);
            }
            else
            {
                return AddInputLayout()
                    .SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT))
                    .SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT))
                    .SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT))
                    .SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
                    .SetRenderTargets({ DXGI_FORMAT_R8G8B8A8_UNORM })
                    .SetDepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT)
                    .SetSampleDesc(UINT_MAX, 1, 0);
            }
            
        }

        PipelineStateBuilder& SetRootSignature(ID3D12RootSignature* rootSig) 
        {
            psoDesc.pRootSignature = rootSig;
            return *this;
        }

        PipelineStateBuilder& SetVertexShader(Shader* vertexShader) 
        {
            psoDesc.VS = { vertexShader->GetShader()->GetBufferPointer(), vertexShader->GetShader()->GetBufferSize() };
            return *this;
        }

        PipelineStateBuilder& SetPixelShader(Shader* pixelShader) 
        {
            psoDesc.PS = { pixelShader->GetShader()->GetBufferPointer(), pixelShader->GetShader()->GetBufferSize() };
            return *this;
        }

        PipelineStateBuilder& SetBlendState(const D3D12_BLEND_DESC& blendState) 
        {
            psoDesc.BlendState = blendState;
            return *this;
        }

        PipelineStateBuilder& SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerState) 
        {
            psoDesc.RasterizerState = rasterizerState;
            return *this;
        }

        PipelineStateBuilder& SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilState) 
        {
            psoDesc.DepthStencilState = depthStencilState;
            return *this;
        }

        PipelineStateBuilder& SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topology) 
        {
            psoDesc.PrimitiveTopologyType = topology;
            return *this;
        }

        PipelineStateBuilder& AddInputLayout(D3D12_INPUT_ELEMENT_DESC* inputLayout = nullptr, UINT count = 4)
        {
            if (inputLayout == nullptr)
                psoDesc.InputLayout = { inputElementDescs, count};
            else
				psoDesc.InputLayout = { inputLayout, count };
            return *this;
        }

        PipelineStateBuilder& SetRenderTargets(std::vector<DXGI_FORMAT> formats)
        {
			for (int i = 0; i < formats.size(); i++)
			{
				psoDesc.RTVFormats[i] = formats[i];
			}
            psoDesc.NumRenderTargets = formats.size();
            return *this;
        }

		PipelineStateBuilder& SetDepthStencilFormat(DXGI_FORMAT format)
		{
			psoDesc.DSVFormat = format;
			return *this;
		}

        PipelineStateBuilder& SetSampleDesc(UINT mask, UINT count, UINT quality)
        {
            DXGI_SAMPLE_DESC sampleDesc{ count, quality };
            psoDesc.SampleMask = mask;
            psoDesc.SampleDesc = sampleDesc;
            return *this;
        }

        D3D12_GRAPHICS_PIPELINE_STATE_DESC Build() 
        {
            return psoDesc;
        }

    private:
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[4] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };
    };

}
