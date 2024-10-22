// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public partial struct Lifecycle
{

	public static Lifecycle ExpiredLifecycle => new(_inlineExpiredToken);
	
}


