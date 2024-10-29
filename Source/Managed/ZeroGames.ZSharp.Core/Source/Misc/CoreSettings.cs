// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public static class CoreSettings
{

	public static bool TreatManagedFatalAsError { get; } = GConfig.GetBoolOrDefault(BaseIniName, Section, nameof(TreatManagedFatalAsError), false);
	public static bool SuppressAlcUnloadedException { get; } = GConfig.GetBoolOrDefault(BaseIniName, Section, nameof(SuppressAlcUnloadedException), true);
	
	private const string BaseIniName = "ZSharp";
	private const string Section = "Managed." + nameof(CoreSettings);

}


