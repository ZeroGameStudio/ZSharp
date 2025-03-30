// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.Core;

internal static class CoreSettings_Interop
{

    public static unsafe delegate* unmanaged[SuppressGCTransition]<uint8> ShouldTreatManagedFatalAsError;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<uint8> ShouldSuppressAlcUnloadedException;

}


