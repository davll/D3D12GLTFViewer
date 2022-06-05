#pragma once

#include "common.h"
#include "command_fence.h"
#include "command_pool.h"
#include "command_queue.h"

namespace mrdr {

class Device {
public:
    struct CreateInfo {
        UINT NumBanks;
        IDXGIAdapter* Adapter;
    };

    Device(const CreateInfo& info);
    ~Device();

    inline ID3D12Device* GetDevice() const;
    inline CommandQueue* GetCommandQueue() const;

    ID3D12GraphicsCommandList* BeginFrame();
    void EndFrame();

private:
    const UINT m_NumBanks;
    UINT m_BankIdx;
    ID3D12Device* m_Device;
    CommandQueue* m_CommandQueue;
    CommandFence* m_Fence;
    CommandPool* m_CommandPool;
    std::vector<UINT64> m_WorkIds;
};

inline ID3D12Device* Device::GetDevice() const
{
    return m_Device;
}

inline CommandQueue* Device::GetCommandQueue() const
{
    return m_CommandQueue;
}

}
