#pragma once

#include "common.h"

namespace mrdr {

class ShaderDescriptorHeap {
public:
    enum class Type : UINT {
        RESOURCE = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        SAMPLER = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
    };
    static_assert((UINT)Type::RESOURCE == 0, "");
    static_assert((UINT)Type::SAMPLER == 1, "");

    struct CreateInfo {
        Type Type;
        UINT MaxNumStaticDescriptors;
        UINT MaxNumDynamicDescriptors;
        ID3D12Device* Device;
    };

    explicit ShaderDescriptorHeap(const CreateInfo& info);
    ~ShaderDescriptorHeap();

    inline Type GetType() const;
    inline UINT GetMaxNumStaticDescriptors() const;
    inline UINT GetMaxNumDynamicDescriptors() const;
    inline UINT GetDescriptorSize() const;
    inline ID3D12DescriptorHeap* GetDescriptorHeap() const;
    inline ShaderDescriptorHandle GetBaseStaticDescriptorHandle() const;

    // TODO: Circular Buffer for Dynamic Descriptors

private:
    void InitDescriptorHeaps();

private:
    const Type m_Type;
    const UINT m_MaxNumStaticDescriptors;
    const UINT m_MaxNumDynamicDescriptors;
    ID3D12Device* m_Device;
    ID3D12DescriptorHeap* m_DescriptorHeap;
    UINT m_DescriptorSize;
    ShaderDescriptorHandle m_BaseStaticDescriptorHandle;
    ShaderDescriptorHandle m_BaseDynamicDescriptorHandle;
};

inline ShaderDescriptorHeap::Type ShaderDescriptorHeap::GetType() const
{
    return m_Type;
}

inline UINT ShaderDescriptorHeap::GetMaxNumStaticDescriptors() const
{
    return m_MaxNumStaticDescriptors;
}

inline UINT ShaderDescriptorHeap::GetMaxNumDynamicDescriptors() const
{
    return m_MaxNumDynamicDescriptors;
}

inline ID3D12DescriptorHeap* ShaderDescriptorHeap::GetDescriptorHeap() const
{
    return m_DescriptorHeap;
}

inline UINT ShaderDescriptorHeap::GetDescriptorSize() const
{
    return m_DescriptorSize;
}

inline ShaderDescriptorHandle ShaderDescriptorHeap::GetBaseStaticDescriptorHandle() const
{
    assert(m_MaxNumStaticDescriptors > 0);
    return m_BaseStaticDescriptorHandle;
}

}
