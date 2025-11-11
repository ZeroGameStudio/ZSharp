// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public partial struct Lifecycle
{

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Lifecycle FromBackend<T>(T backend) where T : class
		=> backend is CancellationTokenSource cts ? new(cts) : new(backend);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Lifecycle FromCancellationTokenSource(CancellationTokenSource cts) => new(cts);

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Lifecycle FromCancellationToken(CancellationToken cancellationToken) => new(cancellationToken);

	public static Lifecycle NeverExpired => default;
	public static Lifecycle Expired => new(true);
	
}


