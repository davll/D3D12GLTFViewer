#include "command_pool.h"

namespace mrdr {

CommandPool::CommandPool(const CreateInfo& info)
: m_Type(info.Type)
, m_NumBanks(info.NumBanks)
, m_NumThreads(info.NumThreads)
, m_CurrBankIdx(0)
, m_Recording(FALSE)
, m_NumRecordedCommandLists(0)
, m_Device(info.Device)
{
    m_Device->AddRef();

    InitCommandAllocators();
}

CommandPool::~CommandPool()
{
    assert(!m_Recording);

    for (ID3D12GraphicsCommandList* o : m_CommandLists)
        o->Release();
    for (ID3D12CommandAllocator* o : m_CommandAllocators)
        o->Release();
    m_Device->Release();
}

void CommandPool::Reset(UINT bankIdx)
{
    assert(bankIdx < m_NumBanks);
    assert(!m_Recording);

    for (ID3D12CommandAllocator* cmdalloc : GetCommandAllocators(bankIdx)) {
        MRDR_CHKHR(
            cmdalloc->Reset()
        );
    }

    m_CurrBankIdx = bankIdx;
    m_NumRecordedCommandLists = 0;
}

gsl::span<ID3D12GraphicsCommandList*> CommandPool::BeginRecord()
{
    assert(!m_Recording);
    m_Recording = TRUE;

    auto cmdallocs = GetCommandAllocators(m_CurrBankIdx);

    UINT off = m_NumRecordedCommandLists;
    UINT nreset = m_CommandLists.size() - off;
    // UINT nnew = m_NumThreads - nreset;
    for (UINT i = 0; i < m_NumThreads; ++i) {
        ID3D12CommandAllocator* cmdalloc = cmdallocs[i];
        if (i < nreset) {
            MRDR_CHKHR(
                m_CommandLists[off+i]->Reset(cmdalloc, NULL)
            );
        } else {
            ID3D12GraphicsCommandList* newcmdlist;
            MRDR_CHKHR(
                m_Device->CreateCommandList(0,
                    (D3D12_COMMAND_LIST_TYPE)(UINT)m_Type,
                    cmdalloc, NULL,
                    IID_PPV_ARGS(&newcmdlist))
            );
            m_CommandLists.push_back(newcmdlist);
        }
    }

    auto base = m_CommandLists.data();
    base += m_NumRecordedCommandLists;
    return gsl::make_span(base, m_NumThreads);
}

void CommandPool::EndRecord()
{
    assert(m_Recording);
    m_Recording = FALSE;

    UINT off = m_NumRecordedCommandLists;
    for (UINT i = 0; i < m_NumThreads; ++i) {
        MRDR_CHKHR(
            m_CommandLists[off+i]->Close()
        );
    }

    m_NumRecordedCommandLists += m_NumThreads;
}

gsl::span<ID3D12CommandList*> CommandPool::GetPendingCommandLists()
{
    assert(!m_Recording);
    auto base = (ID3D12CommandList **) m_CommandLists.data();
    return gsl::make_span(base, m_NumRecordedCommandLists);
}

void CommandPool::InitCommandAllocators()
{
    const auto type = (D3D12_COMMAND_LIST_TYPE)(UINT)m_Type;
    const UINT num = m_NumBanks * m_NumThreads;

    m_CommandAllocators.resize((size_t)num, NULL);

    for (UINT i = 0; i < num; ++i) {
        MRDR_CHKHR(
            m_Device->CreateCommandAllocator(type, IID_PPV_ARGS(&m_CommandAllocators[i]))
        );
    }
}

gsl::span<ID3D12CommandAllocator*> CommandPool::GetCommandAllocators(UINT bankIdx)
{
    ID3D12CommandAllocator** base = m_CommandAllocators.data();
    base += bankIdx * m_NumThreads;
    return gsl::make_span(base, m_NumThreads);
}

}
