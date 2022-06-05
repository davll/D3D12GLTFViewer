#pragma once

#include "common.h"
#include "descriptor_span.h"

namespace mrdr {

class CommandFence;

class GpuDescriptorHeap {
public:
    enum class Type : UINT {
        RESOURCE = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        SAMPLER = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
    };

    struct CreateInfo {
        Type Type;
        UINT NumStaticDescriptors;
        UINT NumDynamicDescriptors;
        ID3D12Device* Device;
        //CommandFence* Fence;
    };

    GpuDescriptorHeap(const CreateInfo& info);
    ~GpuDescriptorHeap();

    inline ID3D12DescriptorHeap* GetDescriptorHeap() const;
    inline CpuDescriptorSpan GetStaticCpuSpan() const;
    inline GpuDescriptorSpan GetStaticGpuSpan() const;
    inline CpuDescriptorSpan GetDynamicCpuSpan() const;
    inline GpuDescriptorSpan GetDynamicGpuSpan() const;

    // DescriptorIndexRange AllocateDynamic(UINT num);

private:
    const Type m_Type;
    const UINT m_NumStatic;
    const UINT m_NumDynamic;
    const UINT m_Size;
    ID3D12DescriptorHeap* m_DescriptorHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_CpuBase;
    D3D12_GPU_DESCRIPTOR_HANDLE m_GpuBase;
};

inline ID3D12DescriptorHeap* GpuDescriptorHeap::GetDescriptorHeap() const
{
    return m_DescriptorHeap;
}

inline CpuDescriptorSpan GpuDescriptorHeap::GetStaticCpuSpan() const
{
    auto base = m_CpuBase;
    base.ptr += m_Size * m_NumDynamic;
    return { base, m_NumStatic, m_Size };
}

inline GpuDescriptorSpan GpuDescriptorHeap::GetStaticGpuSpan() const
{
    auto base = m_GpuBase;
    base.ptr += m_Size * m_NumDynamic;
    return { base, m_NumStatic, m_Size };
}

inline CpuDescriptorSpan GpuDescriptorHeap::GetDynamicCpuSpan() const
{
    return { m_CpuBase, m_NumDynamic, m_Size };
}

inline GpuDescriptorSpan GpuDescriptorHeap::GetDynamicGpuSpan() const
{
    return { m_GpuBase, m_NumDynamic, m_Size };
}

}
