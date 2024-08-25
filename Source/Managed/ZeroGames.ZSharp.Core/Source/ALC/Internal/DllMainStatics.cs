// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal static class DllMainStatics
{

    public static MethodInfo? GetDllMainMethodInfo(Assembly assembly)
    {
        DllEntryAttribute? dllEntryAttr = assembly.GetCustomAttribute<DllEntryAttribute>();
        if (dllEntryAttr is null)
        {
            return null;
        }

        Type dllEntryType = dllEntryAttr.EntryType;
        MethodInfo? dllMain = dllEntryType.GetMethods(BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic)
            .FirstOrDefault(method => method.GetCustomAttribute<DllMainAttribute>() is not null);
        if (dllMain is null)
        {
            throw new Exception($"Assembly {assembly.GetName().Name} has entry type but not entry method!");
        }

        return dllMain;
    }

    public static unsafe ELoadAssemblyErrorCode TryInvokeDllMain(Assembly assembly, void* args)
    {
        MethodInfo? dllMain = GetDllMainMethodInfo(assembly);
        if (dllMain is null)
        {
            return ELoadAssemblyErrorCode.Succeed;
        }

        object?[]? parameters = null;
        ParameterInfo[] parameterInfos = dllMain.GetParameters();
        if (parameterInfos.Length > 0)
        {
            if (parameterInfos.Length != 1)
            {
                return ELoadAssemblyErrorCode.InvalidDllMainSignature;
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
                    return ELoadAssemblyErrorCode.InvalidDllMainSignature;
                }
                        
                parameters = [ new IntPtr(args) ];
            }
        }
        
        dllMain.Invoke(null, parameters);
        
        return ELoadAssemblyErrorCode.Succeed;
    }
    
}


