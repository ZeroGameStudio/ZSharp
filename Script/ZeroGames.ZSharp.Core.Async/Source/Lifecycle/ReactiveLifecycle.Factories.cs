// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ReactiveLifecycle
{
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReactiveLifecycle FromBackend(IReactiveLifecycleBackend backend) => new(backend);

	public static ReactiveLifecycle NeverExpired => default;
	public static ReactiveLifecycle Expired => new(true);

}


