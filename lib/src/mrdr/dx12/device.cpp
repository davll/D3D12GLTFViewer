#include "device.h"

namespace mrdr {
namespace dx12 {

Device::Device(const CreateInfo& info)
: m_NumBanks(info.NumBanks)
, m_BankIdx(0)
, m_WorkIds((size_t)info.NumBanks, 0)
{
    MRDR_CHKHR(
        D3D12CreateDevice(
            info.Adapter,
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_Device))
    );

    m_Fence = new CommandFence({ m_Device, TRUE });

    m_CommandQueue = new CommandQueue({
        CommandQueue::Type::GRAPHICS,
        m_Device,
    });

    m_CommandPool = new CommandPool({
        CommandPool::Type::GRAPHICS,
        m_NumBanks, (UINT)1,
        m_Device,
    });
}

Device::~Device()
{
    m_Fence->CpuWait();
    delete m_Fence;
    delete m_CommandPool;
    delete m_CommandQueue;
    m_Device->Release();
}

ID3D12GraphicsCommandList* Device::BeginFrame()
{
    m_Fence->CpuWait(m_WorkIds[m_BankIdx]);
    m_CommandPool->Reset(m_BankIdx);

    auto recordings = m_CommandPool->BeginRecord();
    return recordings[0];
}

void Device::EndFrame()
{
    m_CommandPool->EndRecord();

    auto pendings = m_CommandPool->GetPendingCommandLists();
    m_CommandQueue->SubmitCommands(pendings);

    UINT64 workId = m_CommandQueue->SubmitSignal(m_Fence);
    m_WorkIds[m_BankIdx] = workId;

    m_BankIdx = (m_BankIdx + 1) % m_NumBanks;
}

}
}
