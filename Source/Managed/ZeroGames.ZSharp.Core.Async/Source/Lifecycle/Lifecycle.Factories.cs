// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public partial struct Lifecycle
{

	public static Lifecycle ExpiredLifecycle => new(_inlineExpiredToken);
	
}


