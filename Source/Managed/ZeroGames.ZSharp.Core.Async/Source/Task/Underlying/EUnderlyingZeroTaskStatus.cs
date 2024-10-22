// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async.ZeroTask;

public enum EUnderlyingZeroTaskStatus
{
	/// <summary>The operation has not yet completed.</summary>
	Pending,
	/// <summary>The operation completed successfully.</summary>
	Succeeded,
	/// <summary>The operation completed with an error.</summary>
	Faulted,
	/// <summary>The operation completed due to cancellation.</summary>
	Canceled,
}


