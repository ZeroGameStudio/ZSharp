// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct SwitchToGameThreadTask : IAwaitable<SwitchToGameThreadTask.Awaiter>
{
	
	public readonly struct Awaiter : IAwaiter, IMoveNextSourceAwaiter
	{
		void IMoveNextSourceAwaiter.OnCompleted(IMoveNextSource source)
			=> GameThreadScheduler.Instance.Post(static state => ((IMoveNextSource)state!).MoveNext(), source);
		
		public void OnCompleted(Action continuation)
			=> GameThreadScheduler.Instance.Post(_ => continuation(), null);
		
		public void GetResult(){}
		
		public bool IsCompleted => !_forceAsync && IsInGameThread;
		
		internal Awaiter(bool forceAsync) => _forceAsync = forceAsync;
		
		private readonly bool _forceAsync;
	}

	public Awaiter GetAwaiter() => new(_forceAsync);

	public SwitchToGameThreadTask ForceAsync() => new(true);

	private SwitchToGameThreadTask(bool forceAsync) => _forceAsync = forceAsync;
	
	private readonly bool _forceAsync;
	
}


