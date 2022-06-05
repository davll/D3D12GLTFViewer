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
        UINT NumBanks;
        UINT NumThreads;
        ID3D12Device* Device;
    };

    CommandPool(const CreateInfo& info);
    ~CommandPool();

    void Reset(UINT bankIdx);
    gsl::span<ID3D12GraphicsCommandList*> BeginRecord();
    void EndRecord();
    gsl::span<ID3D12CommandList*> GetPendingCommandLists();

private:
    void InitCommandAllocators();
    gsl::span<ID3D12CommandAllocator*> GetCommandAllocators(UINT bankIdx);

private:
    const Type m_Type;
    const UINT m_NumBanks;
    const UINT m_NumThreads;
    UINT m_CurrBankIdx;
    BOOL m_Recording;
    UINT m_NumRecordedCommandLists;
    ID3D12Device* m_Device;
    std::vector<ID3D12CommandAllocator*> m_CommandAllocators;
    std::vector<ID3D12GraphicsCommandList*> m_CommandLists;
};

}
