// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public static class CoreSettings
{

	public static unsafe bool TreatManagedFatalAsError => CoreSettings_Interop.ShouldTreatManagedFatalAsError() > 0;
	public static unsafe bool SuppressAlcUnloadedException => CoreSettings_Interop.ShouldSuppressAlcUnloadedException() > 0;

}


