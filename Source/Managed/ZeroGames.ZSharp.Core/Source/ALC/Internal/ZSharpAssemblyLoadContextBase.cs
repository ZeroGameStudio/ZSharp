// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

internal abstract class ZSharpAssemblyLoadContextBase : AssemblyLoadContext, IZSharpAssemblyLoadContext
{
    
    protected ZSharpAssemblyLoadContextBase(string name) : base(name, true)
    {
        GCHandle = GCHandle.Alloc(this);

        Assembly resolverAssembly = Default.Assemblies.Single(asm => asm.GetCustomAttribute<AssemblyResolverAttribute>() is not null);
        Type resolverType = resolverAssembly.GetCustomAttribute<AssemblyResolverAttribute>()!.ResolverType;
        if (!resolverType.IsAssignableTo(typeof(IAssemblyResolver)))
        {
            throw new InvalidOperationException();
        }
        
        _resolver = Unsafe.As<IAssemblyResolver>(Activator.CreateInstance(resolverType)!);
            
        Resolving += HandleResolve;
        Unloading += _ => HandleUnload();
    }
    
    public unsafe ELoadAssemblyErrorCode LoadAssembly(string name, void* args, out Assembly? assembly)
    {
        ELoadAssemblyErrorCode res = _resolver.Resolve(this, name, out assembly);
        if (assembly is null)
        {
            return res;
        }

        try
        {
            res = DllMainStatics.TryInvokeDllMain(assembly, args);
        }
        catch (Exception ex)
        {
            Logger.Error($"Unhandled DllMain Exception:\n{ex}");
            return ELoadAssemblyErrorCode.UnhandledDllMainException;
        }

        return res;
    }

    public unsafe ECallMethodErrorCode CallMethod(string assemblyName, string typeName, string methodName, void* args)
    {
        throw new NotImplementedException();
    }
    
    public GCHandle GCHandle { get; }

    protected virtual void HandleUnload(){}

    private unsafe Assembly? HandleResolve(AssemblyLoadContext alc, AssemblyName name)
    {
        LoadAssembly(name.Name!, null, out var assembly);
        return assembly;
    }

    private IAssemblyResolver _resolver;

}


