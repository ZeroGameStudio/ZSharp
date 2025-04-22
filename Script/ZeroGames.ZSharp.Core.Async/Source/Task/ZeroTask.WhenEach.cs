// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static IZeroStream<T> WhenEach<T>(params IEnumerable<ZeroTask<T>> tasks) => throw new NotImplementedException();
	
}


