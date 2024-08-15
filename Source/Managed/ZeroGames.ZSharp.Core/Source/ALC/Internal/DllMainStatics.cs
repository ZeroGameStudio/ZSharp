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

    public static unsafe bool TryInvokeDllMain(Assembly assembly, void* args, out int32? result)
    {
        result = null;
        
        MethodInfo? dllMain = GetDllMainMethodInfo(assembly);
        if (dllMain is null)
        {
            return false;
        }

        object?[]? parameters = null;
        ParameterInfo[] parameterInfos = dllMain.GetParameters();
        if (parameterInfos.Length > 0)
        {
            if (parameterInfos.Length != 1)
            {
                throw new Exception("DllMain has more than 1 parameters.");
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
                    throw new Exception("DllMain has non-pointer parameter but not string[].");
                }
                        
                parameters = [ new IntPtr(args) ];
            }
        }
                
        object? res = dllMain.Invoke(null, parameters);
        if (res is int32 intRes)
        {
            result = intRes;
        }
        
        return true;
    }
    
}


