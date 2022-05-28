#include "CommandManager.h"

namespace minirdr {

static inline D3D12_COMMAND_LIST_TYPE CommandTypeToD3D12(CommandType ty)
{
    switch (ty) {
        case CommandType::GRAPHICS:
            return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case CommandType::COMPUTE:
            return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case CommandType::TRANSFER:
            return D3D12_COMMAND_LIST_TYPE_COPY;
        default:
            abort();
    }
    return (D3D12_COMMAND_LIST_TYPE)0;
}

CommandManager::CommandManager(ID3D12Device* device, UINT numFrames)
: m_NumFrames(numFrames)
, m_Device(device)
, m_Frames(numFrames)
{
    m_Device->AddRef();

    InitCommandQueues();
    InitCommandFences();

    // Reset counters
    m_SubmittedWorkCounts.fill(0);
    for (PerFrame& frame : m_Frames) {
        frame.SubmittedWorkIds.fill(0);
    }
}

CommandManager::~CommandManager()
{
    for (UINT i = 0; i < m_NumFrames; ++i)
        ResetCommands(i);
    m_Frames.clear();

    for (UINT t = 0; t < (UINT)CommandType::_COUNT; ++t) {
        for (ID3D12CommandAllocator* o : m_FreeCommandAllocators[t]) {
            o->Release();
        }
        m_FreeCommandAllocators[t].clear();

        for (ID3D12GraphicsCommandList* o : m_FreeCommandLists[t]) {
            o->Release();
        }
        m_FreeCommandLists[t].clear();
    }

    for (HANDLE event : m_CommandFenceEvents) {
        CloseHandle(event);
    }

    for (ID3D12Fence* fence : m_CommandFences) {
        fence->Release();
    }

    for (ID3D12CommandQueue* queue : m_CommandQueues) {
        queue->Release();
    }

    m_Device->Release();
}

void CommandManager::ResetCommands(UINT frameIdx)
{
    auto& frame = m_Frames[frameIdx];

    // Wait for GPU
    {
        UINT count = 0;
        HANDLE events[(UINT)CommandType::_COUNT];

        for (UINT t = 0; t < (UINT)CommandType::_COUNT; ++t) {
            UINT64 workId = frame.SubmittedWorkIds[t];
            if (m_CommandFences[t]->GetCompletedValue() < workId) {
                HANDLE event = m_CommandFenceEvents[t];
                MINIRDR_CHKHR(
                    m_CommandFences[t]->SetEventOnCompletion(workId, event)
                );
                events[count++] = event;
            }
        }

        WaitForMultipleObjects(count, events, TRUE, INFINITE);
    }

    // Move command lists and allocators
    for (UINT t = 0; t < (UINT)CommandType::_COUNT; ++t) {
        for (ID3D12GraphicsCommandList* o : frame.UsedCommandLists[t]) {
            m_FreeCommandLists[t].push_back(o);
        }
        frame.UsedCommandLists[t].clear();

        for (ID3D12CommandAllocator* o : frame.UsedCommandAllocators[t]) {
            o->Reset();
            m_FreeCommandAllocators[t].push_back(o);
        }
        frame.UsedCommandAllocators[t].clear();
    }
}

WorkId CommandManager::SubmitCommands(UINT frameIdx, CommandType ty, UINT numWorkloads, const Workload* workloads)
{
    auto& frame = m_Frames[frameIdx];
    UINT64 workId = ++m_SubmittedWorkCounts[(UINT)ty];
    frame.SubmittedWorkIds[(UINT)ty] = workId;

    for (UINT i = 0; i < numWorkloads; ++i) {
        m_CommandQueues[(UINT)ty]->ExecuteCommandLists(workloads[i].NumCommandLists, workloads[i].CommandLists);
    }
    m_CommandQueues[(UINT)ty]->Signal(m_CommandFences[(UINT)ty], workId);

    return workId;
}

ID3D12CommandAllocator* CommandManager::NewCommandAllocator(UINT frameIdx, CommandType ty)
{
    auto& frame = m_Frames[frameIdx];

    ID3D12CommandAllocator* commandAllocator;

    if (m_FreeCommandAllocators[(UINT)ty].empty()) {
        MINIRDR_CHKHR(
            m_Device->CreateCommandAllocator(CommandTypeToD3D12(ty), IID_PPV_ARGS(&commandAllocator))
        );
    } else {
        commandAllocator = m_FreeCommandAllocators[(UINT)ty].front();
        m_FreeCommandAllocators[(UINT)ty].pop_front();
    }
    frame.UsedCommandAllocators[(UINT)ty].push_back(commandAllocator);

    return commandAllocator;
}

ID3D12GraphicsCommandList* CommandManager::NewCommandList(UINT frameIdx, CommandType ty, ID3D12CommandAllocator* commandAllocator)
{
    auto& frame = m_Frames[frameIdx];

    ID3D12GraphicsCommandList* commandList;

    if (m_FreeCommandLists[(UINT)ty].empty()) {
        MINIRDR_CHKHR(
            m_Device->CreateCommandList(0, CommandTypeToD3D12(ty), commandAllocator, NULL, IID_PPV_ARGS(&commandList))
        );
    } else {
        commandList = m_FreeCommandLists[(UINT)ty].front();
        m_FreeCommandLists[(UINT)ty].pop_front();
        MINIRDR_CHKHR(
            commandList->Reset(commandAllocator, NULL)
        );
    }
    frame.UsedCommandLists[(UINT)ty].push_back(commandList);

    return commandList;
}

void CommandManager::InitCommandQueues()
{
    D3D12_COMMAND_QUEUE_DESC desc;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    for (UINT i = 0; i < (UINT)CommandType::_COUNT; ++i) {
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        MINIRDR_CHKHR(
            m_Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueues[i]))
        );
    }
}

void CommandManager::InitCommandFences()
{
    for (UINT i = 0; i < (UINT)CommandType::_COUNT; ++i) {
        MINIRDR_CHKHR(
            m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_CommandFences[i]))
        );
    }

    for (UINT i = 0; i < (UINT)CommandType::_COUNT; ++i) {
        m_CommandFenceEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (!m_CommandFenceEvents[i]) {
            abort();
        }
    }
}

}
