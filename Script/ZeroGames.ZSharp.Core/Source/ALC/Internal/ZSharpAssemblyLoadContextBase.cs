// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

internal abstract class ZSharpAssemblyLoadContextBase : AssemblyLoadContext, IZSharpAssemblyLoadContext
{

    public unsafe ELoadAssemblyErrorCode LoadAssembly(string name, void* args, out Assembly? assembly)
    {
        this.GuardUnloaded();
        
        return InternalLoadAssembly(name, args, out assembly, false);
    }

    public unsafe EInvokeMethodErrorCode InvokeMethod(string assemblyName, string typeName, string methodName, void* args)
    {
        this.GuardUnloaded();
        
        return AssemblyLoadContextHelper.InvokeMethod(this, assemblyName, typeName, methodName, args);
    }
    
    public bool IsUnloaded { get; private set; }
    
    protected ZSharpAssemblyLoadContextBase(string name) : base(name, true)
    {
        _resolver = IAssemblyResolver.Create();
        
        Resolving += HandleResolve;
        Unloading += _ => HandleUnload();
    }

    protected virtual void HandleAssemblyLoaded(Assembly assembly){}

    protected virtual void HandleUnload()
    {
        IsUnloaded = true;
    }
    
    private unsafe ELoadAssemblyErrorCode InternalLoadAssembly(string name, void* args, out Assembly? assembly, bool implicitly)
    {
        ELoadAssemblyErrorCode err = AssemblyLoadContextHelper.LoadAssembly(this, _resolver, implicitly, name, args, out assembly);
        if (err == ELoadAssemblyErrorCode.Succeed)
        {
            HandleAssemblyLoaded(assembly!);
        }
        
        return err;
    }

    private unsafe Assembly? HandleResolve(AssemblyLoadContext alc, AssemblyName name)
    {
        InternalLoadAssembly(name.Name!, null, out var assembly, true);
        return assembly;
    }

    private readonly IAssemblyResolver _resolver;

}


