// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UnrealFieldEmitterHelper
{
    public static unsafe void InternalConstructScriptStructInstance(void* instance, UScriptStruct scriptStruct)
    {
        UnrealFieldEmitter_Interop.InternalConstructScriptStructInstance(instance, (void*)scriptStruct.Unmanaged);
    }
    
    public static unsafe void InternalDestructScriptStructInstance(void* instance, UScriptStruct scriptStruct)
    {
        UnrealFieldEmitter_Interop.InternalDestructScriptStructInstance(instance, (void*)scriptStruct.Unmanaged);
    }
}


