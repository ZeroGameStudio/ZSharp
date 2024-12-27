// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class AsyncSettings
{

	public static bool UseZSharpSynchronizationContext { get; } = GConfig.GetBoolOrDefault(BASE_INI_NAME, SECTION, nameof(UseZSharpSynchronizationContext), true);
	public static bool ForceAsyncZeroTaskMethodContinueOnGameThread { get; } = GConfig.GetBoolOrDefault(BASE_INI_NAME, SECTION, nameof(ForceAsyncZeroTaskMethodContinueOnGameThread), true);
	
	private const string BASE_INI_NAME = "ZSharp";
	private const string SECTION = "Managed." + nameof(AsyncSettings);

}


