// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class Type_Interop
{

    [UnmanagedCallersOnly]
    public static int32 GetName(GCHandle handle, IntPtr outNameAddr) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.Target is Type type)
        {
            InteropString interopStr = new(outNameAddr);
            string fullName = type.FullName!;
            string name = fullName.Split(',')[0];
            interopStr.Data = name;

            return 0;
        }

        return 1;
    }, -1);

    [UnmanagedCallersOnly]
    public static ConjugateHandle BuildConjugate(GCHandle handle, IntPtr unmanaged) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.Target is Type type)
        {
            Type conjugateType = typeof(IConjugate<>).MakeGenericType(type);
            if (type.IsAssignableTo(conjugateType))
            {
                return ConjugateHandle.FromConjugate(
                    (IConjugate)type.GetMethod("BuildConjugate")!.Invoke(null, new object[] { unmanaged })!);
            }
        }

        return default;
    }, default);

    [UnmanagedCallersOnly]
    public static unsafe GCHandle GetMethodInfo(GCHandle handle, char* name) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.Target is Type type)
        {
            MethodInfo? method = type.GetMethod(new string(name));
            if (method is not null)
            {
                return GCHandle.Alloc(method);
            }
        }

        return default;
    }, default);

    [UnmanagedCallersOnly]
    public static unsafe GCHandle GetPropertyInfo(GCHandle handle, char* name) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.Target is Type type)
        {
            PropertyInfo? property = type.GetProperty(new string(name));
            if (property is not null)
            {
                return GCHandle.Alloc(property);
            }
        }

        return default;
    }, default);

}


