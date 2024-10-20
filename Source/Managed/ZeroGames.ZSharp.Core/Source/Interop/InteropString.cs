﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct InteropString : IDisposable
{

    public InteropString() : this(string.Empty){}
    
    public InteropString(string str)
    {
        unsafe
        {
            fixed (char* data = str)
            {
                _address = InteropString_Interop.Alloc(data);
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
                InteropString_Interop.Free(_address);
            }
        }
    }

    public override string ToString() => Data;

    public unsafe string Data
    {
        get => new(InteropString_Interop.GetData(_address));
        set
        {
            fixed (char* data = value)
            {
                InteropString_Interop.SetData(_address, data);
            }
        }
    }

    public IntPtr Address => _address;

    private readonly bool _unmanaged;
    private readonly IntPtr _address;

}


