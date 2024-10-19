// Copyright Zero Games. All Rights Reserved.

using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using Mono.Cecil;

namespace ZeroGames.ZSharp.Core;

internal abstract class ZSharpAssemblyLoadContextBase : AssemblyLoadContext, IZSharpAssemblyLoadContext
{
    
    protected ZSharpAssemblyLoadContextBase(string name) : base(name, true)
    {
        GCHandle = GCHandle.Alloc(this);

        _resolver = IAssemblyResolver.Create();
            
        Resolving += HandleResolve;
        Unloading += _ => HandleUnload();
    }

    public unsafe ELoadAssemblyErrorCode LoadAssembly(string name, void* args, out Assembly? assembly) => InternalLoadAssembly(name, args, out assembly, false);

    public unsafe EInvokeMethodErrorCode InvokeMethod(string assemblyName, string typeName, string methodName, void* args)
    {
        Assembly? assembly;
        try
        {
            assembly = Assemblies.Concat(Default.Assemblies).SingleOrDefault(asm => asm.GetName().Name == assemblyName);
        }
        catch (InvalidOperationException)
        {
            return EInvokeMethodErrorCode.AmbiguousAssemblyName;
        }

        if (assembly is null)
        {
            return EInvokeMethodErrorCode.AssemblyNotFound;
        }

        Type? type = assembly.GetType(typeName);
        if (type is null)
        {
            return EInvokeMethodErrorCode.TypeNotFound;
        }

        MethodInfo? method = type.GetMethod(methodName, BindingFlags.Public | BindingFlags.Static);
        if (method is null)
        {
            return EInvokeMethodErrorCode.MethodNotFound;
        }
        
        object?[]? parameters = null;
        ParameterInfo[] parameterInfos = method.GetParameters();
        if (parameterInfos.Length > 0)
        {
            if (parameterInfos.Length != 1)
            {
                return EInvokeMethodErrorCode.InvalidMethodSignature;
            }
                    
            Type parameterType = parameterInfos[0].ParameterType;
            if (parameterType == typeof(string[]) && args is not null)
            {
                parameters = [ ((CommonMethodArgs*)args)->Parse() ];
            }
            else
            {
                if (!parameterType.IsPointer)
                {
                    return EInvokeMethodErrorCode.InvalidMethodSignature;
                }
                
                parameters = [ new IntPtr(args) ];
            }
        }
        
        method.Invoke(null, parameters);

        return EInvokeMethodErrorCode.Succeed;
    }
    
    public GCHandle GCHandle { get; }

    protected virtual void HandleUnload(){}
    
    private unsafe ELoadAssemblyErrorCode InternalLoadAssembly(string name, void* args, out Assembly? assembly, bool implicitly)
    {
        assembly = null;
        
        string? dllPath = _resolver.Resolve(name);
        if (dllPath is null)
        {
            return ELoadAssemblyErrorCode.FileNotFound;
        }

        if (implicitly)
        {
            try
            {
                using AssemblyDefinition assemblyDef = AssemblyDefinition.ReadAssembly(dllPath);
                foreach (var attr in assemblyDef.CustomAttributes)
                {
                    Thrower.FatalIf(attr.AttributeType.FullName == typeof(DllEntryAttribute).FullName, $"Assembly {assemblyDef.Name.Name} has DllEntry and cannot be loaded implicitly!!!");
                }
            }
            catch (Exception)
            {
                return ELoadAssemblyErrorCode.BadImage;
            }
        }

        using (FileStream fs = File.OpenRead(dllPath))
        {
            try
            {
                assembly = LoadFromStream(fs);
            }
            catch (Exception)
            {
                return ELoadAssemblyErrorCode.BadImage;
            }
        }

        try
        {
            return DllMainStatics.TryInvokeDllMain(assembly, args);
        }
        catch (Exception ex)
        {
            UE_ERROR(LogZSharpScriptCore, $"Unhandled DllMain Exception:\n{ex}");
            return ELoadAssemblyErrorCode.UnhandledDllMainException;
        }
    }

    private unsafe Assembly? HandleResolve(AssemblyLoadContext alc, AssemblyName name)
    {
        InternalLoadAssembly(name.Name!, null, out var assembly, true);
        return assembly;
    }

    private IAssemblyResolver _resolver;

}


