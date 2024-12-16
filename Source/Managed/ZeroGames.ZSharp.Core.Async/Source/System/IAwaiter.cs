// Copyright Zero Games. All Rights Reserved.

namespace System.Runtime.CompilerServices;

public interface IAwaiter : INotifyCompletion
{
	bool IsCompleted { get; }
	void GetResult();
}

public interface IAwaiter<out TResult> : IAwaiter
{
	new TResult GetResult();
}


