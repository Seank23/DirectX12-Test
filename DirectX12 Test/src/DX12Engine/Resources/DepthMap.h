#pragma once
#include "./GPUResource.h"
#include "../Heaps/DescriptorHeapHandle.h"

namespace DX12Engine
{
	class DepthMap : public GPUResource
	{
	public:
		DepthMap(ID3D12Resource* mainResource, D3D12_RESOURCE_STATES usageState, DescriptorHeapHandle shaderDescriptor, std::vector<DescriptorHeapHandle> depthStencilDescriptors, bool isCubeMap = false);
		~DepthMap();

		DescriptorHeapHandle GetDepthStencilDescriptor(int index = 0) { return m_DepthStencilDescriptors[index]; }
		bool GetIsCubeMap() { return m_IsCubeMap; }

	private:
		std::vector<DescriptorHeapHandle> m_DepthStencilDescriptors;
		bool m_IsCubeMap;
	};
}

