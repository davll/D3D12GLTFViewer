#pragma once

#include "common.h"

namespace mrdr {

class CommandPool {
public:
    enum class Type : UINT {
        GRAPHICS = D3D12_COMMAND_LIST_TYPE_DIRECT,
        COMPUTE = D3D12_COMMAND_LIST_TYPE_COMPUTE,
        TRANSFER = D3D12_COMMAND_LIST_TYPE_COPY,
        BUNDLE = D3D12_COMMAND_LIST_TYPE_BUNDLE,
    };

    struct CreateInfo {
        Type Type;
        ID3D12Device* Device;
    };

    explicit CommandPool(const CreateInfo& info);
    ~CommandPool();

    void Reset();

    ID3D12CommandAllocator* NewCommandAllocator();
    ID3D12GraphicsCommandList* NewCommandList(ID3D12CommandAllocator* commandAllocator);

private:
    const Type m_Type;
    ID3D12Device* m_Device;
    std::vector<ID3D12CommandAllocator*> m_CommandAllocators;
    std::vector<ID3D12GraphicsCommandList*> m_CommandLists;
    UINT m_NumUsedCommandAllocators;
    UINT m_NumUsedCommandLists;
};

}
