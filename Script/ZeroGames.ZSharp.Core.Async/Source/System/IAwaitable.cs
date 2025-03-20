// Copyright Zero Games. All Rights Reserved.

namespace System.Runtime.CompilerServices;

public interface IAwaitable<out TAwaiter> where TAwaiter : struct, IAwaiter
{
	TAwaiter GetAwaiter();
}

public interface IAwaitable<TResult, out TAwaiter> : IAwaitable<TAwaiter> where TAwaiter : struct, IAwaiter<TResult>;


