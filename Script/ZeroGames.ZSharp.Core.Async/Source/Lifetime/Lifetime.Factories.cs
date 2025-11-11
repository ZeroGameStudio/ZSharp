// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public partial struct Lifetime
{

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Lifetime FromBackend<T>(T backend) where T : class
		=> backend is CancellationTokenSource cts ? new(cts) : new(backend);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Lifetime FromCancellationTokenSource(CancellationTokenSource cts) => new(cts);

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Lifetime FromCancellationToken(CancellationToken cancellationToken) => new(cancellationToken);

	public static Lifetime NeverExpired => default;
	public static Lifetime Expired => new(true);
	
}


