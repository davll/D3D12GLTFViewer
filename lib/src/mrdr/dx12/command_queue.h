#pragma once

#include "common.h"

namespace mrdr {

class CommandFence;

class CommandQueue {
public:
    enum class Type : UINT {
        GRAPHICS = D3D12_COMMAND_LIST_TYPE_DIRECT,
        COMPUTE = D3D12_COMMAND_LIST_TYPE_COMPUTE,
        TRANSFER = D3D12_COMMAND_LIST_TYPE_COPY,
    };

    struct CreateInfo {
        Type Type;
        ID3D12Device* Device;
    };

    explicit CommandQueue(const CreateInfo& info);
    ~CommandQueue();

    inline ID3D12CommandQueue* GetCommandQueue() const;

    void SubmitCommands(gsl::span<ID3D12CommandList*> commandLists);
    void SubmitWait(CommandFence* fence);
    UINT64 SubmitSignal(CommandFence* fence);

private:
    ID3D12CommandQueue* m_CommandQueue;
};

inline ID3D12CommandQueue* CommandQueue::GetCommandQueue() const
{
    return m_CommandQueue;
}

}
