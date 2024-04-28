// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal static class AssemblyLoadContext_Interop
{

    public static unsafe bool TryInvokeDllMain(Assembly assembly, void* args, out int32? result)
    {
        result = null;
        
        string assemblyName = assembly.FullName!.Split(',')[0];
        Type? entryType = assembly.GetType($"{assemblyName}.__DllEntry");
        if (entryType is not null)
        {
            MethodInfo? dllMain = entryType.GetMethod("DllMain", BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic);
            if (dllMain is not null)
            {
                object?[]? parameters = null;
                ParameterInfo[] parameterInfos = dllMain.GetParameters();
                if (parameterInfos.Length > 0 && args is not null)
                {
                    if (parameterInfos.Length != 1)
                    {
                        throw new Exception("DllMain has more than 1 parameters.");
                    }
                    
                    Type parameterType = parameterInfos[0].ParameterType;
                    if (parameterType == typeof(string[]) && args is not null)
                    {
                        parameters = new object[] { ((CommonDllMainArgs*)args)->Parse() };
                    }
                    else
                    {
                        if (!parameterType.IsPointer)
                        {
                            throw new Exception("DllMain has non-pointer parameter but not string[].");
                        }
                        
                        parameters = new object[] { new IntPtr(args) };
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

        return false;
    }
    
}