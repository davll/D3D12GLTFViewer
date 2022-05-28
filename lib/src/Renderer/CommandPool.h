#pragma once

#include "common.h"
#include <d3d12.h>
#include <vector>

namespace mrdr {

class CommandPool {
public:
    struct CreateInfo {
        D3D12_COMMAND_LIST_TYPE Type;
        ID3D12Device* Device;
    };

    explicit CommandPool(const CreateInfo& info);
    ~CommandPool();

    void Reset();

    ID3D12CommandAllocator* NewCommandAllocator();
    ID3D12GraphicsCommandList* NewCommandList(ID3D12CommandAllocator* commandAllocator);

private:
    const D3D12_COMMAND_LIST_TYPE m_Type;
    ID3D12Device* m_Device;
    std::vector<ID3D12CommandAllocator*> m_CommandAllocators;
    std::vector<ID3D12GraphicsCommandList*> m_CommandLists;
    UINT m_NumUsedCommandAllocators;
    UINT m_NumUsedCommandLists;
};

}
