// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Core;

public readonly unsafe struct InteropString : IDisposable
{

    public InteropString() : this(null){}
    
    public InteropString(string? str)
    {
        _black = true;
        fixed (char* data = str)
        {
            _address = InteropString_Interop.Alloc(data);
        }
    }

    public InteropString(IntPtr address)
    {
        _address = address;
    }

    public void Dispose()
    {
        if (_black)
        {
            InteropString_Interop.Free(_address);
        }
    }

    public override string ToString() => Data;
    
    public static implicit operator string(InteropString value) => value.Data;

    [AllowNull]
    public string Data
    {
        get => _address != IntPtr.Zero ? new(InteropString_Interop.GetData(_address)) : string.Empty;
        set
        {
            if (_address == IntPtr.Zero)
            {
                return;
            }
            
            fixed (char* data = value)
            {
                InteropString_Interop.SetData(_address, data);
            }
        }
    }

    public IntPtr Address => _address;

    private readonly bool _black;
    private readonly IntPtr _address;

}


