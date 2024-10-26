// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public static class CoreSettings
{

	public static bool TreatManagedFatalAsError { get; } = GConfig.GetBoolOrDefault(BaseIniName, Section, nameof(TreatManagedFatalAsError));

	private const string BaseIniName = "ZSharp";
	private const string Section = "CoreSettings";

}


