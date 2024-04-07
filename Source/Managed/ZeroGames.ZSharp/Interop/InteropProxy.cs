using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Interop;

public class InteropProxy<T> : IConjugate where T : class
{
    public InteropProxy(T inner)
    {
        GCHandle = GCHandle.Alloc(this);
        _inner = inner;
    }

    public InteropProxy(T inner, bool bWeak)
    {
        GCHandle = GCHandle.Alloc(this, bWeak ? GCHandleType.Weak : GCHandleType.Normal);
        _inner = inner;
    }

    public void Release()
    {
        GCHandle.Free();
        GCHandle = default;
        _inner = null;
    }

    public GCHandle GCHandle { get; private set; }

    public T Inner
    {
        get
        {
            if (_inner is null)
            {
                throw new Exception();
            }

            return _inner;
        }
    }
    
    private T? _inner;
}