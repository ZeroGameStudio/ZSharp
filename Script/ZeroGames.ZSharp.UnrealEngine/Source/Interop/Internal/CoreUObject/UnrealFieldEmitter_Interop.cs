// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealFieldEmitter_Interop
{
    public static delegate* unmanaged<void*, void*, void> InternalConstructScriptStructInstance;
    public static delegate* unmanaged<void*, void*, void> InternalDestructScriptStructInstance;
    public static delegate* unmanaged<char*, void*, void*, void> InternalReloadStructOpsFakeVTable;
}


