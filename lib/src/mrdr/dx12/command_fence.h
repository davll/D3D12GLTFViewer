#pragma once

#include "common.h"

namespace mrdr {

class CommandQueue;

class CommandFence {
public:
    struct CreateInfo {
        ID3D12Device* Device;
        BOOL CpuWaitEnabled;
    };

    CommandFence(const CreateInfo& info);
    ~CommandFence();

    void CpuWait(UINT64 workId);
    void CpuWait();

    UINT64 GpuSignal(ID3D12CommandQueue* queue);
    void GpuWait(ID3D12CommandQueue* queue);

    BOOL SetWaitEvent(HANDLE event, UINT64 workId);

    inline UINT64 GetPendingWorkId() const;
    inline UINT64 GetNumCompleteWorks() const;

private:
    UINT64 m_NumWorks;
    ID3D12Fence* m_Fence;
    HANDLE m_Event;
};

inline UINT64 CommandFence::GetPendingWorkId() const
{
    return m_NumWorks + 1;
}

inline UINT64 CommandFence::GetNumCompleteWorks() const
{
    return m_Fence->GetCompletedValue();
}

}
