// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class AsyncSettings
{

	public static bool UseZSharpSynchronizationContext => !GConfig.TryGetBool(BaseIniName, Section, nameof(UseZSharpSynchronizationContext), out var value) || value.Value;
	public static bool ForceAsyncZeroTaskMethodContinueOnGameThread => !GConfig.TryGetBool(BaseIniName, Section, nameof(ForceAsyncZeroTaskMethodContinueOnGameThread), out var value) || value.Value;
	
	private const string BaseIniName = "ZSharp";
	private const string Section = "AsyncSettings";

}


