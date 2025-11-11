// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ReactiveLifetime
{
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReactiveLifetime FromBackend(IReactiveLifetimeBackend backend) => new(backend);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReactiveLifetime FromCancellationTokenSource(CancellationTokenSource cts) => new(cts);

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReactiveLifetime FromCancellationToken(CancellationToken cancellationToken) => new(cancellationToken);
	
	public static ReactiveLifetime NeverExpired => default;
	public static ReactiveLifetime Expired => new(true);

}


