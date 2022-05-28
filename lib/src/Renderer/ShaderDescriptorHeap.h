#pragma once

#include "common.h"

namespace mrdr {

class ShaderDescriptorHeap {
public:
    struct CreateInfo {
        D3D12_DESCRIPTOR_HEAP_TYPE Type;
        UINT MaxNumStaticDescriptors;
        UINT MaxNumDynamicDescriptors;
        ID3D12Device* Device;
    };

    explicit ShaderDescriptorHeap(const CreateInfo& info);
    ~ShaderDescriptorHeap();

    inline ID3D12DescriptorHeap* GetDescriptorHeap() const;

    inline UINT GetDescriptorSize() const;

    inline D3D12_CPU_DESCRIPTOR_HANDLE GetStaticDescriptorCpuHandle(UINT idx) const;
    inline D3D12_GPU_DESCRIPTOR_HANDLE GetStaticDescriptorGpuHandle(UINT idx) const;

    // TODO: Circular Buffer for Dynamic Descriptors

private:
    void InitDescriptorHeaps();

private:
    const D3D12_DESCRIPTOR_HEAP_TYPE m_Type;
    const UINT m_MaxNumStaticDescriptors;
    const UINT m_MaxNumDynamicDescriptors;
    ID3D12Device* m_Device;
    ID3D12DescriptorHeap* m_DescriptorHeap;
    UINT m_DescriptorSize;
    D3D12_CPU_DESCRIPTOR_HANDLE m_StaticCpuHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE m_StaticGpuHandle;
    D3D12_CPU_DESCRIPTOR_HANDLE m_DynamicCpuHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE m_DynamicGpuHandle;
};

inline ID3D12DescriptorHeap* ShaderDescriptorHeap::GetDescriptorHeap() const
{
    return m_DescriptorHeap;
}

inline UINT ShaderDescriptorHeap::GetDescriptorSize() const
{
    return m_DescriptorSize;
}

inline D3D12_CPU_DESCRIPTOR_HANDLE ShaderDescriptorHeap::GetStaticDescriptorCpuHandle(UINT idx) const
{
    auto handle = m_StaticCpuHandle;
    handle.ptr += m_DescriptorSize * idx;
    return handle;
}

inline D3D12_GPU_DESCRIPTOR_HANDLE ShaderDescriptorHeap::GetStaticDescriptorGpuHandle(UINT idx) const
{
    auto handle = m_StaticGpuHandle;
    handle.ptr += m_DescriptorSize * idx;
    return handle;
}

}
