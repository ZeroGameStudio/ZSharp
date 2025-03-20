// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal static class CoreSettings_Interop
{

    public static unsafe delegate* unmanaged<uint8> ShouldTreatManagedFatalAsError = null;
    public static unsafe delegate* unmanaged<uint8> ShouldSuppressAlcUnloadedException = null;

}


