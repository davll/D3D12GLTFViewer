#include "CommandQueue.h"

namespace mrdr {

CommandQueue::CommandQueue(const CreateInfo& info)
: m_WorkCount(0)
{
    D3D12_COMMAND_QUEUE_DESC desc;
    desc.Type = (D3D12_COMMAND_LIST_TYPE)(UINT)info.Type;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    MRDR_CHKHR(
        info.Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue))
    );

    MRDR_CHKHR(
        info.Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))
    );
}

CommandQueue::~CommandQueue()
{
    m_Fence->Release();
    m_CommandQueue->Release();
}

void CommandQueue::SubmitCommands(UINT num, ID3D12CommandList* const* pCommandLists)
{
    m_CommandQueue->ExecuteCommandLists(num, pCommandLists);
}

void CommandQueue::SubmitWait(ID3D12Fence* fence, UINT64 workId)
{
    MRDR_CHKHR(
        m_CommandQueue->Wait(fence, workId)
    );
}

UINT64 CommandQueue::SubmitSignal()
{
    const UINT64 workId = ++m_WorkCount;

    MRDR_CHKHR(
        m_CommandQueue->Signal(m_Fence, workId)
    );

    return workId;
}

BOOL CommandQueue::SetWaitEvent(HANDLE event, UINT64 workId)
{
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
