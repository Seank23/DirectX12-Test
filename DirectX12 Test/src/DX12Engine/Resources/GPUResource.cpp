#include "GPUResource.h"

namespace DX12Engine
{
	GPUResource::GPUResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES usageState, DescriptorHeapHandle descriptor)
		: m_Resource(resource), m_UsageState(usageState), m_IsReady(false), m_Descriptor(nullptr)
	{
		m_GPUAddress = m_Resource->GetGPUVirtualAddress();
		m_Descriptor = std::make_unique<DescriptorHeapHandle>(descriptor);
	}

	GPUResource::GPUResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES usageState)
		: m_Resource(resource), m_UsageState(usageState), m_IsReady(false), m_Descriptor(nullptr)
	{
		m_GPUAddress = m_Resource->GetGPUVirtualAddress();
	}

	GPUResource::~GPUResource()
	{
		m_Resource->Release();
	}
}
