#pragma once

#define NOMINMAX
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <intrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <array>
#include <memory>
#include <vector>
#include <deque>

#include <spdlog/spdlog.h>

#include <SDL.h>
#include <imgui.h>
#include <tiny_gltf.h>
#include <DirectXMath.h>

#define MRDR_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define MRDR_FAIL(...) do { SPDLOG_ERROR(__VA_ARGS__); ::mrdr::abort(); } while(0)
#define MRDR_CHKHR(hr) do { if (FAILED(hr)) { ::mrdr::abort(); } } while(0)

namespace mrdr {

struct Size {
    UINT Width;
    UINT Height;
};

struct ObjectDescriptorHandle {
    D3D12_CPU_DESCRIPTOR_HANDLE Cpu;
};

inline ObjectDescriptorHandle operator+(const ObjectDescriptorHandle& a, UINT b)
{
    return { { a.Cpu.ptr + b } };
}

struct ShaderDescriptorHandle {
    D3D12_CPU_DESCRIPTOR_HANDLE Cpu;
    D3D12_GPU_DESCRIPTOR_HANDLE Gpu;
};

inline ShaderDescriptorHandle operator+(const ShaderDescriptorHandle& a, UINT b)
{
    return { { a.Cpu.ptr + b }, { a.Gpu.ptr + b } };
}

#if defined(_DEBUG)
inline void debug_break()
{
    if (IsDebuggerPresent()) {
#if defined(_MSC_VER)
        __debugbreak();
#else
        DebugBreak();
#endif
    }
}
#endif

#if defined(_DEBUG)
inline void abort()
{
    debug_break();
    ::abort();
}
#else
inline void abort()
{
    ::abort();
}
#endif

} // namespace mrdr
