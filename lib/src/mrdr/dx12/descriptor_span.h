#pragma once

#include "common.h"

namespace mrdr {
namespace dx12 {

struct DescriptorIndexRange {
    UINT Base;
    UINT Num;
};

struct CpuDescriptorSpan {
    D3D12_CPU_DESCRIPTOR_HANDLE Base;
    UINT Num;
    UINT Size;

    inline D3D12_CPU_DESCRIPTOR_HANDLE operator[](UINT i) const
    {
        assert(i < Num);
        return { Base.ptr + Size * i };
    }

    inline CpuDescriptorSpan operator[](const DescriptorIndexRange& range) const
    {
        assert(range.Base < Num);
        assert(range.Base + range.Num <= Num);
        auto base = operator[](range.Base);
        UINT num = range.Num;
        return { base, num, Size };
    }
};

struct GpuDescriptorSpan {
    D3D12_GPU_DESCRIPTOR_HANDLE Base;
    UINT Num;
    UINT Size;

    inline D3D12_GPU_DESCRIPTOR_HANDLE operator[](UINT i) const
    {
        assert(i < Num);
        return { Base.ptr + Size * i };
    }

    inline GpuDescriptorSpan operator[](const DescriptorIndexRange& range) const
    {
        assert(range.Base < Num);
        assert(range.Base + range.Num <= Num);
        auto base = operator[](range.Base);
        UINT num = range.Num;
        return { base, num, Size };
    }
};

}
}
