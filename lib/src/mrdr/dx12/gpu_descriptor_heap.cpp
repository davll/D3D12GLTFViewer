#include "gpu_descriptor_heap.h"

namespace mrdr {
namespace dx12 {

GpuDescriptorHeap::GpuDescriptorHeap(const CreateInfo& info)
: m_Type(info.Type)
, m_NumStatic(info.NumStaticDescriptors)
, m_NumDynamic(info.NumDynamicDescriptors)
, m_Size(info.Device->GetDescriptorHandleIncrementSize((D3D12_DESCRIPTOR_HEAP_TYPE)info.Type))
{
    D3D12_DESCRIPTOR_HEAP_DESC desc;
    desc.Type = (D3D12_DESCRIPTOR_HEAP_TYPE)(UINT)m_Type;
    desc.NumDescriptors = m_NumStatic + m_NumDynamic;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;

    MRDR_CHKHR(
        info.Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_DescriptorHeap))
    );

    m_CpuBase = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    m_GpuBase = m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
}

GpuDescriptorHeap::~GpuDescriptorHeap()
{
    m_DescriptorHeap->Release();
}

}
}
