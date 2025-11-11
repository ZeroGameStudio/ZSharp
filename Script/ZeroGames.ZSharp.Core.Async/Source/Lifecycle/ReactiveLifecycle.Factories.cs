// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ReactiveLifecycle
{
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReactiveLifecycle FromBackend(IReactiveLifecycleBackend backend) => new(backend);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReactiveLifecycle FromCancellationTokenSource(CancellationTokenSource cts) => new(cts);

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ReactiveLifecycle FromCancellationToken(CancellationToken cancellationToken) => new(cancellationToken);
	
	public static ReactiveLifecycle NeverExpired => default;
	public static ReactiveLifecycle Expired => new(true);

}


