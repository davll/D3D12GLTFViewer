#include "command_fence.h"
#include "command_queue.h"

namespace mrdr {
namespace dx12 {

CommandFence::CommandFence(const CreateInfo& info)
: m_NumWorks(0)
, m_Fence(NULL)
, m_Event(NULL)
{
    MRDR_CHKHR(
        info.Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))
    );

    if (info.CpuWaitEnabled) {
        m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (!m_Event) {
            abort();
        }
    }
}

CommandFence::~CommandFence()
{
    if (m_Event)
        CloseHandle(m_Event);
    m_Fence->Release();
}

void CommandFence::CpuWait(UINT64 workId)
{
    assert(m_Event);
    
    if (SetWaitEvent(m_Event, workId)) {
        WaitForSingleObject(m_Event, INFINITE);
    }
}

void CommandFence::CpuWait()
{
    CpuWait(m_NumWorks);
}

UINT64 CommandFence::GpuSignal(ID3D12CommandQueue* queue)
{
    UINT64 workId = ++m_NumWorks;

    MRDR_CHKHR(
        queue->Signal(m_Fence, workId)
    );

    return workId;
}

void CommandFence::GpuWait(ID3D12CommandQueue* queue)
{
    MRDR_CHKHR(
        queue->Wait(m_Fence, m_NumWorks)
    );
}

BOOL CommandFence::SetWaitEvent(HANDLE event, UINT64 workId)
{
    assert(workId <= m_NumWorks);

    if (m_Fence->GetCompletedValue() < workId) {
        MRDR_CHKHR(
            m_Fence->SetEventOnCompletion(workId, event)
        );
        return TRUE;
    } else {
        return FALSE;
    }
}

}
}
