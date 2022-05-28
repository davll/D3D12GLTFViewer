#pragma once

#include "common.h"

namespace minirdr {

class CommandQueue {
public:
    struct CreateInfo {
        D3D12_COMMAND_LIST_TYPE Type;
        ID3D12Device* Device;
    };

    explicit CommandQueue(const CreateInfo& info);
    ~CommandQueue();

    inline ID3D12CommandQueue* GetCommandQueue() const;
    inline ID3D12Fence* GetFence() const;
    inline UINT64 GetWorkCount() const;

    void SubmitCommands(UINT num, ID3D12CommandList* const* pCommandLists);
    void SubmitWait(ID3D12Fence* fence, UINT64 workId);
    UINT64 SubmitSignal();

    BOOL SetWaitEvent(HANDLE event, UINT64 workId);

private:
    ID3D12CommandQueue* m_CommandQueue;
    ID3D12Fence* m_Fence;
    UINT64 m_WorkCount;
};

inline ID3D12CommandQueue* CommandQueue::GetCommandQueue() const
{
    return m_CommandQueue;
}

inline ID3D12Fence* CommandQueue::GetFence() const
{
    return m_Fence;
}

inline UINT64 CommandQueue::GetWorkCount() const
{
    return m_WorkCount;
}

}
