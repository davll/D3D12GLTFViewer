#include "CommandPool.h"

namespace mrdr {

CommandPool::CommandPool(const CreateInfo& info)
: m_Type(info.Type)
, m_Device(info.Device)
, m_NumUsedCommandAllocators(0)
, m_NumUsedCommandLists(0)
{
    m_Device->AddRef();
}

CommandPool::~CommandPool()
{
    for (ID3D12CommandAllocator* o : m_CommandAllocators) {
        o->Release();
    }
    for (ID3D12GraphicsCommandList* o : m_CommandLists) {
        o->Release();
    }
    m_Device->Release();
}

void CommandPool::Reset()
{
    m_NumUsedCommandAllocators = 0;
    m_NumUsedCommandLists = 0;
}

ID3D12CommandAllocator* CommandPool::NewCommandAllocator()
{
    ID3D12CommandAllocator* commandAllocator;
    if (m_CommandAllocators.size() > m_NumUsedCommandAllocators) {
        commandAllocator = m_CommandAllocators[m_NumUsedCommandAllocators++];
    } else {
        MRDR_CHKHR(
            m_Device->CreateCommandAllocator(m_Type, IID_PPV_ARGS(&commandAllocator))
        );
        m_CommandAllocators.push_back(commandAllocator);
    }
    return commandAllocator;
}

ID3D12GraphicsCommandList* CommandPool::NewCommandList(ID3D12CommandAllocator* commandAllocator)
{
    ID3D12GraphicsCommandList* commandList;
    if (m_CommandLists.size() > m_NumUsedCommandLists) {
        commandList = m_CommandLists[m_NumUsedCommandLists++];
        MRDR_CHKHR(
            commandList->Reset(commandAllocator, NULL)
        );
    } else {
        MRDR_CHKHR(
            m_Device->CreateCommandList(0, m_Type, commandAllocator, NULL, IID_PPV_ARGS(&commandList))
        );
        m_CommandLists.push_back(commandList);
    }
    return commandList;
}

}
