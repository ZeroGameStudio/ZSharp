// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

// Helps to avoid allocating an Action object when works inside AsyncZeroTaskMethodBuilder.
public interface IMoveNextSourceAwaiter
{
	void OnCompleted(IMoveNextSource source);
}


