// Copyright Zero Games. All Rights Reserved.

using System.IO;
using System.Reflection;
using System.Runtime.Loader;
using Mono.Cecil;

namespace ZeroGames.ZSharp.Core;

internal static class AssemblyLoadContextHelper
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
            return TryInvokeDllMain(assembly, args);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex, "Unhandled DllMain exception.");
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
        
        if (!TryGetParameters(out var parameters, method, args))
        {
            return EInvokeMethodErrorCode.InvalidMethodSignature;
        }
        
        method.Invoke(null, parameters);

        return EInvokeMethodErrorCode.Succeed;
	}
    
    private static unsafe bool TryGetParameters(out object?[]? parameters, MethodInfo method, void* args)
    {
        parameters = null;
        
        ParameterInfo[] parameterInfos = method.GetParameters();
        if (parameterInfos.Length > 0)
        {
            if (parameterInfos.Length != 1)
            {
                return false;
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
                    return false;
                }
                
                parameters = [ new IntPtr(args) ];
            }
        }

        return true;
    }

    private static unsafe ELoadAssemblyErrorCode TryInvokeDllMain(Assembly assembly, void* args)
    {
        DllEntryAttribute? dllEntryAttr = assembly.GetCustomAttribute<DllEntryAttribute>();
        if (dllEntryAttr is null)
        {
            return ELoadAssemblyErrorCode.Succeed;
        }

        Type dllEntryType = dllEntryAttr.EntryType;
        MethodInfo? dllMain = dllEntryType.GetMethods(BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic)
            .FirstOrDefault(method => method.GetCustomAttribute<DllMainAttribute>() is not null);
        if (dllMain is null)
        {
            return ELoadAssemblyErrorCode.DllMainNotFound;
        }
        
        if (!TryGetParameters(out var parameters, dllMain, args))
        {
            return ELoadAssemblyErrorCode.InvalidDllMainSignature;
        }
        
        dllMain.Invoke(null, parameters);
        
        return ELoadAssemblyErrorCode.Succeed;
    }
	
}


