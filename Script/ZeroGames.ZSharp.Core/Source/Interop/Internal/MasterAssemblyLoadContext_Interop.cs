// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static unsafe class MasterAssemblyLoadContext_Interop
{
    
    [UnmanagedCallersOnly]
    public static int32 PrepareUnloading()
    {
        try
        {
            MasterAssemblyLoadContext.Instance!.PrepareUnloading();
            return 0;
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return -1;
        }
    }

    [UnmanagedCallersOnly]
    public static int32 Unload()
    {
        try
        {
            MasterAssemblyLoadContext.Instance!.Unload();
            return 0;
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return -1;
        }
    }

    [UnmanagedCallersOnly]
    public static ELoadAssemblyErrorCode LoadAssembly(char* assemblyName, void* args)
    {
        try
        {
            return MasterAssemblyLoadContext.Instance!.LoadAssembly(new(assemblyName), args, out _);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return ELoadAssemblyErrorCode.UnknownError;
        }
    }
    
    [UnmanagedCallersOnly]
    public static EInvokeMethodErrorCode InvokeMethod(char* assemblyName, char* typeName, char* methodName, void* args)
    {
        try
        {
            return MasterAssemblyLoadContext.Instance!.InvokeMethod(new(assemblyName), new(typeName), new(methodName), args);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return EInvokeMethodErrorCode.UnknownError;
        }
    }

    [UnmanagedCallersOnly]
    public static InteropRuntimeTypeHandle GetType(InteropRuntimeTypeUri uri)
    {
        if (uri._uri is null)
        {
            return default;
        }
        
        try
        {
            return new InteropRuntimeTypeHandle(MasterAssemblyLoadContext.Instance!.GetType(new(uri)));
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return default;
        }
    }

    [UnmanagedCallersOnly]
    public static EZCallErrorCode ZCall_Red(ZCallHandle handle, ZCallBuffer* buffer, IntPtr fatalMessageBuffer)
    {
        try
        {
            MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Instance;
            if (alc is null)
            {
                return EZCallErrorCode.AlcUnavailable;
            }

            return alc.ZCall_Red(handle, buffer);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex, fatalMessageBuffer);
            return EZCallErrorCode.UnknownError;
        }
    }
    
    [UnmanagedCallersOnly]
    public static ZCallHandle GetZCallHandle_Red(char* name)
    {
        try
        {
            return MasterAssemblyLoadContext.Instance!.GetZCallHandle_Red(new string(name));
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return default;
        }
    }

    [UnmanagedCallersOnly]
    public static IntPtr BuildConjugate_Red(IntPtr unmanaged, InteropRuntimeTypeHandle type)
    {
        try
        {
            IntPtr handle = type.Handle;
            if (handle == IntPtr.Zero)
            {
                return IntPtr.Zero;
            }

            return MasterAssemblyLoadContext.Instance!.BuildConjugate_Red(unmanaged, type.Handle);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return IntPtr.Zero;
        }
    }
    
    [UnmanagedCallersOnly]
    public static void ReleaseConjugate_Red(IntPtr unmanaged)
    {
        try
        {
            MasterAssemblyLoadContext.Instance!.ReleaseConjugate_Red(unmanaged);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
        }
    }
    
    public static delegate* unmanaged<ZCallHandle, ZCallBuffer*, EZCallErrorCode> ZCall_Black;
    public static delegate* unmanaged<char*, ZCallHandle> GetZCallHandle_Black;
    public static delegate* unmanaged<uint16, IntPtr, IntPtr> BuildConjugate_Black;
    public static delegate* unmanaged<uint16, IntPtr, void> ReleaseConjugate_Black;

}


