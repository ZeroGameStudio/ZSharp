// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ReactiveLifecycle
{
	
	public static ReactiveLifecycle FromUnderlyingLifecycle(IReactiveUnderlyingLifecycle underlyingLifecycle) => new(underlyingLifecycle);

	public static ReactiveLifecycle NeverExpired => default;
	public static ReactiveLifecycle Expired => new(true);

}


