// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ReactiveLifecycle
{

	public static ReactiveLifecycle ExpiredLifecycle => new(_inlineExpiredToken);
	public static ReactiveLifecycle Explicit(IExplicitLifecycle explicitLifecycle) => new(explicitLifecycle);
	
}


