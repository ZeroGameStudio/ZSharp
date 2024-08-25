// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct InteropString : IDisposable
{

    public InteropString() : this(string.Empty){}
    
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
        _unmanaged = true;
        _address = address;
    }

    public void Dispose()
    {
        if (!_unmanaged)
        {
            unsafe
            {
                InteropString_Interop.SFree(_address);
            }
        }
    }

    public override string ToString() => Data;

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

    public IntPtr Address => _address;

    private readonly bool _unmanaged;
    private readonly IntPtr _address;

}


