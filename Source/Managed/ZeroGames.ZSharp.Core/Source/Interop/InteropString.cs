// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct InteropString : IDisposable
{
    
    public InteropString(string str)
    {
        unsafe
        {
            fixed (char* data = str.ToCharArray())
            {
                _address = InteropString_Interop.SAlloc(data);
            }
        }
    }

    public InteropString(IntPtr address)
    {
        _native = true;
        _address = address;
    }

    public void Dispose()
    {
        if (!_native)
        {
            unsafe
            {
                InteropString_Interop.SFree(_address);
            }
        }
    }

    public unsafe string Data
    {
        get => new(InteropString_Interop.SGetData(_address));
        set
        {
            fixed (char* data = value.ToCharArray())
            {
                InteropString_Interop.SSetData(_address, data);
            }
        }
    }

    private readonly bool _native;
    private readonly IntPtr _address;

}


