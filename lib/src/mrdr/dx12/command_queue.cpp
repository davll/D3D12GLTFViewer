#include "command_queue.h"
#include "command_fence.h"

namespace mrdr {
namespace dx12 {

CommandQueue::CommandQueue(const CreateInfo& info)
{
    D3D12_COMMAND_QUEUE_DESC desc;
    desc.Type = (D3D12_COMMAND_LIST_TYPE)(UINT)info.Type;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    MRDR_CHKHR(
        info.Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue))
    );
}

CommandQueue::~CommandQueue()
{
    m_CommandQueue->Release();
}

void CommandQueue::SubmitCommands(gsl::span<ID3D12CommandList*> commandLists)
{
    m_CommandQueue->ExecuteCommandLists(commandLists.size(), commandLists.data());
}

void CommandQueue::SubmitWait(CommandFence* fence)
{
    fence->GpuWait(m_CommandQueue);
}

UINT64 CommandQueue::SubmitSignal(CommandFence* fence)
{
    return fence->GpuSignal(m_CommandQueue);
}

}
}
