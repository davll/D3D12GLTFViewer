#include "ShaderDescriptorHeap.h"

namespace minirdr {

ShaderDescriptorHeap::ShaderDescriptorHeap(const CreateInfo& info)
: m_Type(info.Type)
, m_MaxNumStaticDescriptors(info.MaxNumStaticDescriptors)
, m_MaxNumDynamicDescriptors(info.MaxNumDynamicDescriptors)
, m_Device(info.Device)
{
    m_Device->AddRef();
    m_DescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    InitDescriptorHeaps();
}

ShaderDescriptorHeap::~ShaderDescriptorHeap()
{
    m_DescriptorHeap->Release();
    m_Device->Release();
}

void ShaderDescriptorHeap::InitDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc;
    desc.Type = m_Type;
    desc.NumDescriptors = m_MaxNumStaticDescriptors + m_MaxNumDynamicDescriptors;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;

    MINIRDR_CHKHR(
        m_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_DescriptorHeap))
    );

    m_StaticCpuHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    m_StaticGpuHandle = m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart();

    m_DynamicCpuHandle = m_StaticCpuHandle;
    m_DynamicCpuHandle.ptr += m_DescriptorSize * m_MaxNumStaticDescriptors;

    m_DynamicGpuHandle = m_StaticGpuHandle;
    m_DynamicGpuHandle.ptr += m_DescriptorSize * m_MaxNumStaticDescriptors;
}

}
