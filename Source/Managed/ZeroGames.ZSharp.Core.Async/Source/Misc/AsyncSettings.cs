// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class AsyncSettings
{

	public static bool UseZSharpSynchronizationContext { get; } = GConfig.GetBoolOrDefault(BaseIniName, Section, nameof(UseZSharpSynchronizationContext), true);
	public static bool ForceAsyncZeroTaskMethodContinueOnGameThread { get; } = GConfig.GetBoolOrDefault(BaseIniName, Section, nameof(ForceAsyncZeroTaskMethodContinueOnGameThread), true);
	
	private const string BaseIniName = "ZSharp";
	private const string Section = "AsyncSettings";

}


