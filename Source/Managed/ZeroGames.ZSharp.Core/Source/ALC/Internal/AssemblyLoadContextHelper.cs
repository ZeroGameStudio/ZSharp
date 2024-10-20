// Copyright Zero Games. All Rights Reserved.

using System.IO;
using System.Reflection;
using System.Runtime.Loader;
using Mono.Cecil;

namespace ZeroGames.ZSharp.Core;

public static class AssemblyLoadContextHelper
{
	
	public static unsafe ELoadAssemblyErrorCode LoadAssembly(AssemblyLoadContext alc, IAssemblyResolver resolver, bool implicitly, string name, void* args, out Assembly? assembly)
    {
        assembly = alc.Assemblies.SingleOrDefault(asm => asm.GetName().Name == name);
        if (assembly is not null)
        {
            return ELoadAssemblyErrorCode.Succeed;
        }

        string? dllPath = resolver.Resolve(name);
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
                assembly = alc.LoadFromStream(fs);
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

	public static unsafe EInvokeMethodErrorCode InvokeMethod(AssemblyLoadContext alc, string assemblyName, string typeName, string methodName, void* args)
	{
        Assembly? assembly;
        try
        {
            assembly = alc.Assemblies.Concat(alc != AssemblyLoadContext.Default ? AssemblyLoadContext.Default.Assemblies : []).SingleOrDefault(asm => asm.GetName().Name == assemblyName);
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
	
}


