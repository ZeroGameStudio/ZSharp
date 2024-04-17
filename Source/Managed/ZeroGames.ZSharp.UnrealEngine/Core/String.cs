using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.UnrealEngine;

public class String : IConjugate<String>
{
    
    public static String? BuildConjugate(IntPtr unmanaged)
    {
        return null;
    }

    public String() : this(string.Empty)
    {
        
    }

    public String(string content)
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _bNative = false;
    }

    public override string ToString()
    {
        return "123";
    }

    public GCHandle GCHandle { get; }
    public IntPtr Unmanaged { get; }
    
    private String(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        _bNative = true;
    }
    
    ~String()
    {
        if (!_bNative)
        {
            
        }
    }

    private bool _bNative;

}


