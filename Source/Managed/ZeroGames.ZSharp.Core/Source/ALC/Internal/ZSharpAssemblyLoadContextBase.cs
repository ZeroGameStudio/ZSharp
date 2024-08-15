// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

internal abstract class ZSharpAssemblyLoadContextBase : AssemblyLoadContext, IZSharpAssemblyLoadContext
{
    
    protected ZSharpAssemblyLoadContextBase(string name) : base(name, true)
    {
        GCHandle = GCHandle.Alloc(this);

        Unloading += _ => HandleUnload();
    }

    protected virtual void HandleUnload()
    {

    }
    
    public GCHandle GCHandle { get; }
    
}


