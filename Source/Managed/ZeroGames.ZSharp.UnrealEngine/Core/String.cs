using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.UnrealEngine;

public class String : IConjugate<String>
{
    
    public static String? BuildConjugate(IntPtr unmanaged)
    {
        return null;
    }

    public String()
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _bNative = false;
    }

    ~String()
    {
        if (!_bNative)
        {
            
        }
    }

    private String(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        _bNative = true;
    }

    public GCHandle GCHandle { get; }
    public IntPtr Unmanaged { get; }

    private bool _bNative;

}


