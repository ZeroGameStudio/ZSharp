// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

internal interface IStreamingTask
{
	void Update(int32 loadedCount);
	void SignalCompletion();
}


