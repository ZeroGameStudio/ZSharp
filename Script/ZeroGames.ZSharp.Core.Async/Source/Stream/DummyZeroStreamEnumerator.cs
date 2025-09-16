// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct DummyZeroStreamEnumerator<T> : IZeroStreamEnumerator<T>
{
    #region IZeroStreamEnumerator<T> Implementations
        
    public ZeroTask DisposeAsync() => ZeroTask.CompletedTask;
    public ZeroTask<bool> MoveNextAsync() => ZeroTask.FromResult(false);
    public T Current => throw new InvalidOperationException();

    #endregion
}


