// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

internal abstract class ZSharpAssemblyLoadContextBase : AssemblyLoadContext, IZSharpAssemblyLoadContext
{

    public unsafe ELoadAssemblyErrorCode LoadAssembly(string name, void* args, out Assembly? assembly) => InternalLoadAssembly(name, args, out assembly, false);

    public unsafe EInvokeMethodErrorCode InvokeMethod(string assemblyName, string typeName, string methodName, void* args)
    {
        return AssemblyLoadContextHelper.InvokeMethod(this, assemblyName, typeName, methodName, args);
    }
    
    public GCHandle GCHandle { get; }
    
    protected ZSharpAssemblyLoadContextBase(string name) : base(name, true)
    {
        GCHandle = GCHandle.Alloc(this);

        _resolver = IAssemblyResolver.Create();
            
        Resolving += HandleResolve;
        Unloading += _ => HandleUnload();
    }

    protected virtual void HandleUnload(){}
    
    private unsafe ELoadAssemblyErrorCode InternalLoadAssembly(string name, void* args, out Assembly? assembly, bool implicitly)
    {
        return AssemblyLoadContextHelper.LoadAssembly(this, _resolver, implicitly, name, args, out assembly);
    }

    private unsafe Assembly? HandleResolve(AssemblyLoadContext alc, AssemblyName name)
    {
        InternalLoadAssembly(name.Name!, null, out var assembly, true);
        return assembly;
    }

    private readonly IAssemblyResolver _resolver;

}


