#pragma once

#include "common.h"
#include <d3d12.h>
#include <array>
#include <vector>
#include <deque>

namespace mrdr {

class CommandManager {
public:
    explicit CommandManager(ID3D12Device* device, UINT numFrames);
    ~CommandManager();

    inline ID3D12Device* GetDevice() const;
    inline ID3D12CommandQueue* GetCommandQueue(CommandType ty) const;
    inline ID3D12Fence* GetCommandFence(CommandType ty) const;

    // void SubmitWait(UINT frameIdx, CommandType to, CommandType from);

    void ResetCommands(UINT frameIdx);
    WorkId SubmitCommands(UINT frameIdx, CommandType ty, UINT numWorkloads, const Workload* workloads);
    ID3D12CommandAllocator* NewCommandAllocator(UINT frameIdx, CommandType ty);
    ID3D12GraphicsCommandList* NewCommandList(UINT frameIdx, CommandType ty, ID3D12CommandAllocator* commandAllocator);

private:
    void InitCommandQueues();
    void InitCommandFences();

private:
    const UINT m_NumFrames;

    ID3D12Device* m_Device;
    std::array<ID3D12CommandQueue*, (UINT)CommandType::_COUNT> m_CommandQueues;
    std::array<ID3D12Fence*, (UINT)CommandType::_COUNT> m_CommandFences;
    std::array<HANDLE, (UINT)CommandType::_COUNT> m_CommandFenceEvents;
    std::array<UINT64, (UINT)CommandType::_COUNT> m_SubmittedWorkCounts;

    std::array<std::deque<ID3D12CommandAllocator*>, (UINT)CommandType::_COUNT> m_FreeCommandAllocators;
    std::array<std::deque<ID3D12GraphicsCommandList*>, (UINT)CommandType::_COUNT> m_FreeCommandLists;

    struct PerFrame {
        std::array<std::vector<ID3D12CommandAllocator*>, (UINT)CommandType::_COUNT> UsedCommandAllocators;
        std::array<std::vector<ID3D12GraphicsCommandList*>, (UINT)CommandType::_COUNT> UsedCommandLists;
        std::array<UINT64, (UINT)CommandType::_COUNT> SubmittedWorkIds;
    };
    std::vector<PerFrame> m_Frames;
};

inline ID3D12Device* CommandManager::GetDevice() const
{
    return m_Device;
}

inline ID3D12CommandQueue* CommandManager::GetCommandQueue(CommandType ty) const
{
    return m_CommandQueues[(UINT)ty];
}

inline ID3D12Fence* CommandManager::GetCommandFence(CommandType ty) const
{
    return m_CommandFences[(UINT)ty];
}

}
