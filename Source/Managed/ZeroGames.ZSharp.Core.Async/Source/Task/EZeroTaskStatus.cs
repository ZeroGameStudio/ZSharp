// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public enum EZeroTaskStatus
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


