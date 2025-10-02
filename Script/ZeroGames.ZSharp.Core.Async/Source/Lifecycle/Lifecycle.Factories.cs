// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public partial struct Lifecycle
{
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Lifecycle FromBackend(ILifecycleBackend backend) => new(backend);

	public static Lifecycle NeverExpired => default;
	public static Lifecycle Expired => new(true);
	
}


